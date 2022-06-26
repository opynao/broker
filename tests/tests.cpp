#include "message.h"
#include "message_broker.h"
#include "message_publisher.h"
#include "logger.h"
#include "../common/include/error_codes.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <chrono>
#include <string>
#include <thread>
#include <vector>
#include <functional>
#include <random>
struct MockPingPonger : public IMessageListener<MessageType>
{
    MockPingPonger(const MessageType &msgResponse)
        : m_msgResponse{msgResponse}
    {
    }
    virtual void OnMessageReceived(const MessageType &msg) override
    {
        //... do some stuff
        if (msg.GetMessageHead().replyTo.empty())
            return;
        auto b = Ensety::MessageBroker::MessageBrokerImpl::GetInstance();
        Ensety::MessageBroker::MessageBrokerImpl::GetInstance()->Publish(m_msgResponse, msg.GetMessageHead().replyTo);
    }

private:
    MessageType m_msgResponse;
};

class MessageListenerTest : public ::testing::Test
{
public:
    inline static const std::string notExistQueueName{"NOT_EXIST"};
    inline static const std::string simpleMessage{"SIMPLE_MESSAGE"};
    inline static const std::string simpleRK{"SIMPLE_RK"};
    inline static const std::string serviceQueue{"SERVICE_QUEUE_FIRST"};
    inline static const std::string replyToQueue{"REPLY_TO_QUEUE"};

protected:
    MessageListenerTest()
        : spMessageBroker{Ensety::MessageBroker::MessageBrokerImpl::GetInstance()},
          msgResponse{MessageFactoryType::CreateMessage({"", ""}, simpleMessage)}
    {
    }
    void SetUp() override
    {
        spMockPingPongMessageListener = std::make_shared<MockPingPonger>(msgResponse);
        spMessageBroker->RegisterQueue({serviceQueue, ""});
        spMessageBroker->BindQueueWithListener(serviceQueue, spMockPingPongMessageListener);
    }

    std::shared_ptr<Ensety::MessageBroker::IMessageBroker> spMessageBroker;
    MessageType msgResponse;
    std::shared_ptr<MockPingPonger> spMockPingPongMessageListener;
};
template <typename T>
class MockMessageListener : public IMessageListener<T>
{
public:
    MOCK_METHOD(void, OnMessageReceived, (const T &), (override));
};

TEST_F(MessageListenerTest, testOnMessageReceived)
{
    auto spMockMessageListener = std::make_shared<MockMessageListener<MessageType>>();
    const auto msg = MessageFactoryType::CreateMessage({"", ""}, simpleMessage);
    EXPECT_CALL(*spMockMessageListener, OnMessageReceived(msg)).Times(1);

    spMessageBroker->RegisterQueue({serviceQueue, ""});
    spMessageBroker->BindQueueWithListener(serviceQueue, spMockMessageListener);
    EXPECT_EQ(spMessageBroker->Publish(msg, serviceQueue), Ensety::ErrorCodes::Success);
}

TEST_F(MessageListenerTest, testReplyTo)
{
    auto spMockResponseListener = std::make_shared<MockMessageListener<MessageType>>();
    spMessageBroker->RegisterQueue({replyToQueue, ""});
    spMessageBroker->BindQueueWithListener(replyToQueue, spMockResponseListener);

    EXPECT_CALL(*spMockResponseListener, OnMessageReceived(msgResponse)).Times(1);

    // At first message goes to first queue, here it process, after doing all necessary stuff
    // Message listener checks whether he should send reply
    const auto msg = MessageFactoryType::CreateMessage({"", replyToQueue}, simpleMessage);
    EXPECT_EQ(spMessageBroker->Publish(msg, serviceQueue), Ensety::ErrorCodes::Success);
}

TEST_F(MessageListenerTest, testPublishToNonExistQueue)
{
    auto spMockMessageListener = std::make_shared<MockMessageListener<MessageType>>();
    const auto msg = MessageFactoryType::CreateMessage({"", ""}, simpleMessage);

    EXPECT_EQ(Ensety::ErrorCodes::MessageBroker_QueueNotExistError, spMessageBroker->Publish(msg, notExistQueueName));
}

TEST_F(MessageListenerTest, testOnMessageReceivedAllQueueListeneres)
{
    auto spMockMessageListenerFirst = std::make_shared<MockMessageListener<MessageType>>();
    auto spMockMessageListenerSecond = std::make_shared<MockMessageListener<MessageType>>();
    const auto msg = MessageFactoryType::CreateMessage({"", ""}, simpleMessage);
    EXPECT_CALL(*spMockMessageListenerFirst, OnMessageReceived(msg)).Times(1);
    EXPECT_CALL(*spMockMessageListenerSecond, OnMessageReceived(msg)).Times(1);

    spMessageBroker->RegisterQueue({serviceQueue, ""});
    spMessageBroker->BindQueueWithListener(serviceQueue, spMockMessageListenerFirst);

    EXPECT_EQ(spMessageBroker->BindQueueWithListener(serviceQueue, spMockMessageListenerSecond), true);
    EXPECT_EQ(spMessageBroker->Publish(msg, serviceQueue), Ensety::ErrorCodes::Success);
}

TEST_F(MessageListenerTest, testBindError_QueueNotRegisterBefore)
{
    auto spMockMessageListener = std::make_shared<MockMessageListener<MessageType>>();

    EXPECT_EQ(spMessageBroker->BindQueueWithListener(notExistQueueName, spMockMessageListener), false);
}

TEST_F(MessageListenerTest, testBindSeveralTimes_OnMessageREceivedOneTime)
{
    auto spMockMessageListener = std::make_shared<MockMessageListener<MessageType>>();
    const auto msg = MessageFactoryType::CreateMessage({"", ""}, simpleMessage);
    EXPECT_CALL(*spMockMessageListener, OnMessageReceived(msg)).Times(1);

    spMessageBroker->RegisterQueue({serviceQueue, ""});
    spMessageBroker->BindQueueWithListener(serviceQueue, spMockMessageListener);

    for (int i = 0; i != 3; ++i)
        EXPECT_EQ(spMessageBroker->BindQueueWithListener(serviceQueue, spMockMessageListener), true);

    EXPECT_EQ(spMessageBroker->Publish(msg, serviceQueue), Ensety::ErrorCodes::Success);
}

TEST_F(MessageListenerTest, testRegisterQueue_QueueAlreadyExists)
{
    EXPECT_EQ(spMessageBroker->RegisterQueue({notExistQueueName, ""}), Ensety::ErrorCodes::Success);
    EXPECT_EQ(spMessageBroker->RegisterQueue({notExistQueueName, ""}), Ensety::ErrorCodes::MessageBroker_QueueAlreadyExists);
}

TEST_F(MessageListenerTest, testUnbindMessageListener_OnMessageReceivedDoesntCall)
{
    auto spMockMessageListener = std::make_shared<MockMessageListener<MessageType>>();
    QueueParams replyQueue{"REPLY_QUEUE", ""};
    spMessageBroker->RegisterQueue(replyQueue);

    spMessageBroker->BindQueueWithListener(replyQueue.queueName, spMockMessageListener);
    spMessageBroker->UnbindListenerFromQueue(replyQueue.queueName, spMockMessageListener);
    const auto msg = MessageFactoryType::CreateMessage({"", ""}, simpleMessage);

    EXPECT_CALL(*spMockMessageListener, OnMessageReceived(msg)).Times(0);
    EXPECT_EQ(spMessageBroker->Publish(msg, serviceQueue), Ensety::ErrorCodes::Success);
}

TEST_F(MessageListenerTest, testSendMessagesOnlyQueuesWithRoutingKeys)
{
    auto spMockMessageListenerFirst = std::make_shared<MockMessageListener<MessageType>>();
    auto spMockMessageListenerSecond = std::make_shared<MockMessageListener<MessageType>>();
    auto spMockMessageListenerThird = std::make_shared<MockMessageListener<MessageType>>();

    const auto msg = MessageFactoryType::CreateMessage({simpleRK, ""}, simpleMessage);
    EXPECT_CALL(*spMockMessageListenerFirst, OnMessageReceived(msg)).Times(1);
    EXPECT_CALL(*spMockMessageListenerSecond, OnMessageReceived(msg)).Times(1);
    EXPECT_CALL(*spMockMessageListenerThird, OnMessageReceived(msg)).Times(0);

    QueueParams queueFirst{"QUEUE_FIRST", simpleRK};
    QueueParams queueSecond{"QUEUE_SECOND", simpleRK};
    QueueParams queueWithoutRK{"QUEUE_THIRD", ""};

    spMessageBroker->RegisterQueue(queueFirst);
    spMessageBroker->RegisterQueue(queueSecond);
    spMessageBroker->RegisterQueue(queueWithoutRK);

    // The third queue is without rKey, so messages are gonna get only first and second queues

    spMessageBroker->BindQueueWithListener(queueFirst.queueName, spMockMessageListenerFirst);
    spMessageBroker->BindQueueWithListener(queueSecond.queueName, spMockMessageListenerSecond);
    spMessageBroker->BindQueueWithListener(queueWithoutRK.queueName, spMockMessageListenerThird);

    EXPECT_EQ(spMessageBroker->Publish(msg), Ensety::ErrorCodes::Success);
}

TEST(MessagePublisherTest, testPublish)
{
    static const std::string messageBrokerServiceQueueName{"messageBrokerServiceQueueName"};
    static const std::string simpleMessage{"SIMPLE_MESSAGE"};
    QueueParams simpleQueueFirst{"simple_queue_first", ""};
    QueueParams simpleQueueSecond{"simple_queue_second", ""};

    auto spMessageBroker = Ensety::MessageBroker::MessageBrokerImpl::GetInstance();
    MessagePublisher messagePublisher(*spMessageBroker);

    auto spMockMessageListenerFirst = std::make_shared<MockMessageListener<MessageType>>();
    auto spMockMessageListenerSecond = std::make_shared<MockMessageListener<MessageType>>();

    const auto msg = MessageFactoryType::CreateMessage({"", ""}, simpleMessage);

    EXPECT_CALL(*spMockMessageListenerFirst, OnMessageReceived(msg)).Times(2);
    EXPECT_CALL(*spMockMessageListenerSecond, OnMessageReceived(msg)).Times(2);

    spMessageBroker->RegisterQueue(simpleQueueFirst);
    spMessageBroker->BindQueueWithListener(simpleQueueFirst.queueName, spMockMessageListenerFirst);
    messagePublisher.AddQueue(simpleQueueFirst.queueName);
    messagePublisher.Publish(msg);

    spMessageBroker->RegisterQueue(simpleQueueSecond);
    spMessageBroker->BindQueueWithListener(simpleQueueSecond.queueName, spMockMessageListenerSecond);
    messagePublisher.AddQueue(simpleQueueSecond.queueName);
    messagePublisher.Publish(msg);

    messagePublisher.RemoveQueue(simpleQueueFirst.queueName);
    messagePublisher.Publish(msg);
}
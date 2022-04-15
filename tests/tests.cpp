#include "message.h"
#include "message_broker.h"
#include "logger.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

class MockFirst : public Observer<MessageType>
{
public:
    MOCK_METHOD(void, OnMessageReceived, (const MessageType &), (override));
};
class MockSecond : public Observer<MessageType>
{
public:
    MOCK_METHOD(void, OnMessageReceived, (const MessageType &), (override));
};

class MockQueue : public Observable<MessageType>
{
public:
    MOCK_METHOD(void, Subscribe, (const std::shared_ptr<Observer<MessageType>> &));
};

TEST(SingleMessageConsumerSingleMessageProducer, MsgReachesConsumer)
{
    auto &broker{MessageBroker::GetInstance()};

    auto mockFirst = std::make_shared<MockFirst>();
    auto mockSecond = std::make_shared<MockSecond>();
    broker.QueueRegister("mock_first", "mock.first", mockFirst);
    broker.QueueRegister("mock_second", "mock.second", mockSecond);

    auto msg1 = MessageFactoryType::CreateMessage({"mock.first", "mock.second"}, "mock first sends msg to mock second");
    EXPECT_CALL(*mockFirst, OnMessageReceived(msg1)).Times(1);

    broker.Publish(msg1);
    
    ASSERT_TRUE(testing::Mock::VerifyAndClearExpectations(&mockFirst));
}
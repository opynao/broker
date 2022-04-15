#include "message.h"
#include "message_broker.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

TEST(SingleMessageConsumerSingleMessageProducer, MsgReachesConsumer)
{
    auto broker = MessageBroker::GetInstance();

    auto logger = std::make_shared<Logger>();
    broker->QueueRegister("logger_queue", "logger.queue", logger);

    auto msg1 = MessageFactoryType::CreateMessage({"logger.queue", "gui.queue"}, "gui sends to logger");
    broker->Publish(msg1); // gui sends to logger
}
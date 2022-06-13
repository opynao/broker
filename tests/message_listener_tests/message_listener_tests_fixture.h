#ifndef __MESSAGE_LISTENER_TESTS_FIXTURE_H__
#define __MESSAGE_LISTENER_TESTS_FIXTURE_H__

#include "message.h"
#include "message_broker.h"
#include "logger.h"
#include "../common/include/error_codes.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

// class MessageListenerTest : public ::testing::Test
// {
// protected:
//     MessageListenerTest()
//         : msgResponse{MessageFactoryType::CreateMessage({"", ""}, "reply message")}
//     {
//     }
//     void SetUp() override
//     {
//         spMessageBroker = Ensety::MessageBroker::MessageBrokerImpl::GetInstance();
//         spMockPingPongMessageListener = std::make_shared<MockPingPonger>(msgResponse);
//         Ensety::MessageBroker::MessageBrokerImpl::GetInstance()->RegisterQueue("PING_PONG_QUEUE", spMockPingPongMessageListener);
//     }

//     std::shared_ptr<Ensety::MessageBroker::IMessageBroker> spMessageBroker;
//     MessageType msgResponse;
//     std::shared_ptr<MockPingPonger> spMockPingPongMessageListener;
// };
#endif
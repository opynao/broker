#pragma once
#include <string>
#include <memory>
#define TODO true
using routingKey_t = std::string;
using replyTo_t = std::string;
using queueName_t = std::string;

struct MessageHead
{
    routingKey_t rKey;
    replyTo_t replyTo;
    static_assert(TODO, "MAKE MOVE CONSTRYCTOR");
    // queueName_t queueName;
};

using MessageBody = std::string;

template <typename MessageHeadType, typename MessageBodyType>
class Message
{
public:
    Message(MessageHeadType messageHead, MessageBodyType messageBody)
        : m_messageHead{messageHead}, m_messageBody{messageBody}
    {
    }
    MessageHeadType GetMessageHead() const
    {
        return m_messageHead;
    }
    MessageBodyType GetMessageBody() const
    {
        return m_messageBody;
    }

private:
    MessageHeadType m_messageHead;
    MessageBodyType m_messageBody;
};

template <typename MessageHeadType, typename MessageBodyType>
struct MessageFactory
{
    using MessageType = Message<MessageHeadType, MessageBodyType>;
    static MessageType CreateMessage(MessageHeadType messageHead, MessageBodyType messageBody)
    {
        return MessageType(messageHead, messageBody);
    }
};
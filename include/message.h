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
    friend bool operator==(const MessageHead &msgHeadFirst, const MessageHead &msgHeadSecond)
    {
        return msgHeadFirst.rKey == msgHeadSecond.rKey && msgHeadFirst.replyTo == msgHeadSecond.replyTo;
    }
    // queueName_t queueName;
};

// using MessageBody = std::string;

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
    friend bool operator==(const Message<MessageHeadType, MessageBodyType> &msgFirst, const Message<MessageHeadType, MessageBodyType> &msgSecond)
    {
        return msgFirst.m_messageHead == msgSecond.m_messageHead && msgFirst.m_messageBody == msgSecond.m_messageBody;
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

using MessageFactoryType = MessageFactory<MessageHead, std::string>;
using MessageType = MessageFactoryType::MessageType;
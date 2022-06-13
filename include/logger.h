#ifndef __LOGGER_H__
#define __LOGGER_H__
#include "message.h"
#include "message_listener.h"
#include "utils.h"

using namespace Ensety::MessageBroker;
class Logger : public IMessageListener<MessageType>
{
public:
    virtual void OnMessageReceived(const MessageType &msg) override
    {
        std::cout << msg.GetMessageBody() << std::endl;
    }
};

class GUI : public IMessageListener<MessageType>
{
public:
    virtual void OnMessageReceived(const MessageType &msg) override
    {
        std::cout << msg.GetMessageBody() << std::endl;
    }
};

#endif
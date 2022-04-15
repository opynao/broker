#pragma once
#include "message.h"
#include "observer.h"
#include "utils.h"

class Logger : public Observer<MessageType>
{
public:
    virtual void OnMessageReceived(const MessageType &msg) override
    {
        std::cout << msg.GetMessageBody() << std::endl;
    }
};

class GUI : public Observer<MessageType>
{
public:
    virtual void OnMessageReceived(const MessageType &msg) override
    {
        std::cout << msg.GetMessageBody() << std::endl;
    }
};
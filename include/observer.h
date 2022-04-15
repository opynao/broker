#pragma once
#include "message.h"
#include "utils.h"

#include <iostream>
#include <vector>

class Observer
{
public:
    virtual void OnMessageReceived(const MessageType &msg) = 0;
    virtual ~Observer() {}
};

class Observable
{
public:
    void Subscribe(std::shared_ptr<Observer> f)
    {
        observers.push_back(f);
    }
    void Unsubscribe(std::shared_ptr<Observer> f)
    {
        // ...
    }

protected:
    void Notify(const MessageType &msg)
    {
        for (auto obs : observers)
            if (obs)
                obs->OnMessageReceived(msg);
    }

private:
    std::vector<std::shared_ptr<Observer>> observers;
};
class Logger : public Observer
{
public:
    virtual void OnMessageReceived(const MessageType &msg) override
    {
        std::cout << msg.GetMessageBody() << std::endl;
    }
};

class GUI : public Observer
{
public:
    virtual void OnMessageReceived(const MessageType &msg) override
    {
        std::cout << msg.GetMessageBody() << std::endl;
    }
};
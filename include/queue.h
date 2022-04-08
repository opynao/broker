#pragma once
#include "observer.h"

#include <queue>

class Queue : public Observable
{
public:
    Queue(std::string name)
        : queueName{name}
    {
    }
    void Push(const MessageType &spMsg)
    {
        q.push(spMsg);
        Notify(spMsg);
    }
    void Pop()
    {
        q.pop();
    }

private:
    std::queue<MessageType> q;
    std::string queueName;
};



#ifndef __OBSERVABLE_H__
#define __OBSERVABLE_H__
#include "message.h"
#include "message_listener.h"

#include <iostream>
#include <vector>
#include <set>

namespace Ensety::MessageBroker
{
    template <typename Type>
    class Observable
    {
    public:
        void Subscribe(const std::shared_ptr<IMessageListener<Type>> &observer)
        {
            observers.insert(observer);
        }
        void Unsubscribe(const std::shared_ptr<IMessageListener<Type>> &observer)
        {
            observers.erase(observers.find(observer));
        }

    protected:
        void Notify(const Type &msg)
        {
            for (auto obs : observers)
                if (obs)
                    obs->OnMessageReceived(msg);
        }

    private:
        std::set<std::shared_ptr<IMessageListener<Type>>> observers;
    };
} // !namespace Ensety::MessageBroker
#endif
#ifndef __MESSAGE_PUBLISHER_H__
#define __MESSAGE_PUBLISHER_H__

#include "message.h"

#include <string>
#include <set>

namespace Ensety::MessageBroker
{
    class IMessageBroker;

    class MessagePublisher
    {
    public:
        using queueName_t = std::string;
        MessagePublisher(IMessageBroker &);
        void AddQueue(queueName_t);
        void Publish(const MessageType &);
        void RemoveQueue(queueName_t);

    private:
        std::set<queueName_t> queues;
        IMessageBroker &m_rMessageBroker;
    };
} // !namespace Ensety::MessageBroker
#endif
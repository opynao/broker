#include "message_publisher.h"
#include "message_broker.h"

namespace Ensety::MessageBroker
{

    MessagePublisher::MessagePublisher(IMessageBroker &rMessageBroker)
        : m_rMessageBroker{rMessageBroker}
    {
    }

    void MessagePublisher::AddQueue(queueName_t queueName)
    {
        queues.insert(std::move(queueName));
    }

    void MessagePublisher::Publish(const MessageType &msg)
    {
        for (auto &queueName : queues)
            m_rMessageBroker.Publish(msg, queueName);
    }

    void MessagePublisher::RemoveQueue(queueName_t queueName)
    {
        queues.erase(queueName);
    }
} // !namespace Ensety::MessageBroker

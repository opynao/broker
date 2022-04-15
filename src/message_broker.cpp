#include "message_broker.h"

void MessageBroker::QueueRegister(const queueName_t &queueName, const routingKey_t &rKey, const std::shared_ptr<Observer<MessageType>> &spQueueConsumer)
{
    auto queue = std::make_shared<ThreadsafeQueue<MessageType>>(queueName);
    queue_names.insert({queueName, queue});
    queue_rKey.insert({rKey, queue});
    queue->Subscribe(spQueueConsumer);
    // queues.insert({queueName, spObservable});
}

void MessageBroker::Publish(const MessageType &msg)
{
    auto it = queue_rKey.find(msg.GetMessageHead().rKey);
    if (it != queue_rKey.cend())
    {
        queue_rKey[msg.GetMessageHead().rKey]->Push(msg);
    }
    else
    {
        auto message = MessageFactoryType::CreateMessage({"logger.queue"}, "No queue with rKey " + msg.GetMessageHead().rKey);
        Publish(message);
    }
}

void MessageBroker::Publish(const MessageType &, const queueName_t &)
{
}
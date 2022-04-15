#include "queue.h"
#include "message.h"
#include "utils.h"
#include "observer.h"

#include <memory>
#include <unordered_map>
#include <string>
#include <utility>
#include <queue>
#include <iostream>

class IMessageBroker
{
public:
    virtual void QueueRegister(queueName_t, routingKey_t, std::shared_ptr<Observer>) = 0;
    virtual void ObserverRegister(queueName_t, std::shared_ptr<Observer>) = 0;
    virtual void Publish(const MessageType &spMsg) = 0;
    virtual ~IMessageBroker() {}
};

class MessageBroker : public IMessageBroker
{
public:
    MessageBroker(MessageBroker const &) = delete;
    void operator=(MessageBroker const &) = delete;

    static MessageBroker *GetInstance()
    {
        if (!m_messageBroker)
            return new MessageBroker();
        return m_messageBroker;
    }

    virtual void QueueRegister(queueName_t queueName, routingKey_t rKey, std::shared_ptr<Observer> spQueueConsumer) override
    {
        auto queue = std::make_shared<ThreadsafeQueue<MessageType>>(queueName);
        queue_names.insert({queueName, queue});
        queue_rKey.insert({rKey, queue});
        queue->Subscribe(spQueueConsumer);
        //queues.insert({queueName, spObservable});
    }
    virtual void ObserverRegister(queueName_t queueName, std::shared_ptr<Observer> spObserver) override
    {
        auto it = queue_names.find(queueName);

        if (it != queue_names.cend())
        {
            it->second->Subscribe(spObserver);
        }
        else
        {
            // QueueRegister(queueName, "");
            queue_names[queueName]->Subscribe(spObserver);
        }
    }
    virtual void Publish(const MessageType &msg) override
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

    ~MessageBroker()
    {
        delete m_messageBroker;
    }

private:
    MessageBroker() {}
    std::unordered_map<queueName_t, std::shared_ptr<ThreadsafeQueue<MessageType>>> queue_names;
    std::unordered_map<routingKey_t, std::shared_ptr<ThreadsafeQueue<MessageType>>> queue_rKey;
    std::unordered_map<queueName_t, std::shared_ptr<Observable>> queues;
    static MessageBroker *m_messageBroker;
};

MessageBroker *MessageBroker::m_messageBroker{nullptr};
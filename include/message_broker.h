#include "queue.h"
#include "message.h"
#include "utils.h"
#include "observer.h"
#include "singleton.h"

#include <memory>
#include <unordered_map>
#include <string>
#include <utility>
#include <queue>
#include <iostream>

class IMessageBroker
{
public:
    virtual void QueueRegister(const queueName_t &, const routingKey_t &, const std::shared_ptr<Observer<MessageType>> &) = 0;
    virtual void Publish(const MessageType &) = 0;
    virtual void Publish(const MessageType &, const queueName_t &) = 0;
    virtual ~IMessageBroker() {}
};

class MessageBroker : public IMessageBroker, public Singleton<MessageBroker>
{
public:
    virtual void QueueRegister(const queueName_t &, const routingKey_t &, const std::shared_ptr<Observer<MessageType>> &) override;
    virtual void Publish(const MessageType &) override;
    virtual void Publish(const MessageType &, const queueName_t &) override;

private:
    std::unordered_map<queueName_t, std::shared_ptr<ThreadsafeQueue<MessageType>>> queue_names;
    std::unordered_map<routingKey_t, std::shared_ptr<ThreadsafeQueue<MessageType>>> queue_rKey;
    std::unordered_map<queueName_t, std::shared_ptr<Observable<MessageType>>> queues;
};
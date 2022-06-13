#ifndef __MESSAGE_BROKER_H__
#define __MESSAGE_BROKER_H__
#include "message.h"
#include "utils.h"
#include "message_listener.h"
#include "observerable.h"
#include "queue_params.h"

#include "../common/include/error_codes.h"
#include "../common/include/singleton.h"

#include <memory>
#include <unordered_map>
#include <string>
#include <utility>
#include <queue>
#include <iostream>
#include <set>
#include <optional>
#include <map>

namespace Ensety::MessageBroker
{
    class MessagePublisher;

    class IMessageBroker
    {
    public:
        using listenersList_t = std::vector<std::weak_ptr<IMessageListener<MessageType>>>;

        virtual ErrorCodes RegisterQueue(QueueParams) = 0;
        // virtual void UnregisterQueue(const queueName_t &) = 0;
        virtual bool BindQueueWithListener(queueName_t, std::weak_ptr<IMessageListener<MessageType>>) = 0;
        virtual bool UnbindListenerFromQueue(queueName_t, std::weak_ptr<IMessageListener<MessageType>>) = 0;
        virtual ErrorCodes Publish(const MessageType &) = 0;
        virtual ErrorCodes Publish(const MessageType &, const queueName_t &queueName) = 0;
        virtual bool IsQueueExist(const queueName_t &) const = 0;
        virtual listenersList_t GetListenersForQueue(const queueName_t &) const = 0;

        virtual ~IMessageBroker() {}
    };

    class MessageBrokerImpl : public IMessageBroker, public Common::Singleton<MessageBrokerImpl>
    {
    public:
        virtual ErrorCodes RegisterQueue(QueueParams) override;
        // virtual ErrorCodes UnregisterQueue(const queueName_t &) override;
        virtual bool BindQueueWithListener(queueName_t, std::weak_ptr<IMessageListener<MessageType>>) override;
        virtual bool UnbindListenerFromQueue(queueName_t, std::weak_ptr<IMessageListener<MessageType>>) override;
        virtual ErrorCodes Publish(const MessageType &) override;
        virtual ErrorCodes Publish(const MessageType &, const queueName_t &queueName) override;
        virtual bool IsQueueExist(const queueName_t &) const override;
        virtual listenersList_t GetListenersForQueue(const queueName_t &) const override;

    private:
        std::unordered_map<queueName_t, listenersList_t> queueNameToMessageListener;
        std::unordered_map<routingKey_t, std::shared_ptr<MessagePublisher>> routingKeyToPublisher;

    private:
        std::optional<listenersList_t::const_iterator> IsQueueListenerExist(const queueName_t &,
                                                                            const std::weak_ptr<IMessageListener<MessageType>> &) const;
        void AddQueue(QueueParams);
        bool IsRoutingKeyEmpty(const MessageType &);
        void SendMessageToQueueListeners(const queueName_t &, const MessageType &);
        void AddQueueToPublisher(QueueParams);

        // std::unordered_map<routingKey_t, std::shared_ptr<ThreadsafeQueue<MessageType>>> queue_rKey;
        // std::unordered_map<queueName_t, std::shared_ptr<Observable<MessageType>>> queues;
    };
} // !namespace Ensety::MessageBroker
#endif
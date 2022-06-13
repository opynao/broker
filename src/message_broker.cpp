#include "message_listener.h"
#include "message_broker.h"
#include "utils.h"
#include "message_publisher.h"

#include <algorithm>
#include <ranges>
#include <string>
#include <regex>

namespace Ensety::MessageBroker
{
    [[nodiscard]] ErrorCodes MessageBrokerImpl::RegisterQueue(QueueParams queueParams)
    {
        if (IsQueueExist(queueParams.queueName))
            return ErrorCodes::MessageBroker_QueueAlreadyExists;

        AddQueue(queueParams);
        return ErrorCodes::Success;
    }

    void MessageBrokerImpl::AddQueue(QueueParams queueParams)
    {
        queueNameToMessageListener.insert({queueParams.queueName, {}});
        if (!queueParams.routingKey.empty())
            AddQueueToPublisher(queueParams);
    }

    void MessageBrokerImpl::AddQueueToPublisher(QueueParams queueParams)
    {
        auto routingKey = queueParams.routingKey;
        if (!routingKeyToPublisher.contains(routingKey))
            routingKeyToPublisher.emplace(routingKey, std::make_unique<MessagePublisher>(*this));

        routingKeyToPublisher[routingKey]->AddQueue(queueParams.queueName);
    }

    // ErrorCodes MessageBrokerImpl::UnregisterQueue(const queueName_t &queueName)
    // {
    //     auto it = queueNameToMessageListener.find(queueName);
    //     if (it == queueNameToMessageListener.cend())
    //         return ErrorCodes::MessageBroker_QueueNotExistError;
    //     queueNameToMessageListener.erase(it);
    //     return ErrorCodes::Success;
    // }

    bool MessageBrokerImpl::BindQueueWithListener(queueName_t queueName, std::weak_ptr<IMessageListener<MessageType>> spMessageListener)
    {
        if (!IsQueueExist(queueName))
            return false;

        if (!IsQueueListenerExist(queueName, spMessageListener))
            queueNameToMessageListener[std::move(queueName)].push_back(spMessageListener);
        return true;
    }

    bool MessageBrokerImpl::UnbindListenerFromQueue(queueName_t queueName, std::weak_ptr<IMessageListener<MessageType>> spMessageListener)
    {
        auto queueListenerPos = IsQueueListenerExist(queueName, spMessageListener);
        if (!IsQueueExist(queueName) || !queueListenerPos)
            return true;

        auto listeners = GetListenersForQueue(queueName);
        listeners.erase(queueListenerPos.value());
        return true;
    }

    std::optional<MessageBrokerImpl::listenersList_t::const_iterator> MessageBrokerImpl::IsQueueListenerExist(const queueName_t &queueName,
                                                                                                              const std::weak_ptr<IMessageListener<MessageType>> &spMessageListener) const
    {
        const auto listeners = GetListenersForQueue(queueName);
        const auto it = std::find_if(listeners.cbegin(), listeners.cend(), [&spMessageListener](const auto &spMListener)
                                     { return spMessageListener.lock() == spMListener.lock(); });

        return it != listeners.cend() ? std::optional<MessageBrokerImpl::listenersList_t::const_iterator>{it} : std::nullopt;
    }

    bool MessageBrokerImpl::IsQueueExist(const queueName_t &queueName) const
    {
        return queueNameToMessageListener.find(queueName) != queueNameToMessageListener.cend();
    }

    [[nodiscard]] ErrorCodes MessageBrokerImpl::Publish(const MessageType &msg)
    {
        if (IsRoutingKeyEmpty(msg))
            return ErrorCodes::MessageBroker_EmptyRoutingKey;

        const auto &routingKey = msg.GetMessageHead().rKey;
        auto itPublisher = routingKeyToPublisher.find(routingKey);
        if (itPublisher != routingKeyToPublisher.cend() && itPublisher->second)
            itPublisher->second->Publish(msg);
        else
            [[deprecated("publisher can be absent")]];

        return ErrorCodes::Success;
    }

    [[nodiscard]] ErrorCodes MessageBrokerImpl::Publish(const MessageType &msg, const queueName_t &queueName)
    {
        if (!IsQueueExist(queueName))
            return ErrorCodes::MessageBroker_QueueNotExistError;

        SendMessageToQueueListeners(queueName, msg);
        return ErrorCodes::Success;
    }

    void MessageBrokerImpl::SendMessageToQueueListeners(const queueName_t &queueName, const MessageType &msg)
    {
        for (auto &spMessageListener : GetListenersForQueue(queueName))
        {
            if (auto sp = spMessageListener.lock())
                sp->OnMessageReceived(msg);
        }
    }

    bool MessageBrokerImpl::IsRoutingKeyEmpty(const MessageType &msg)
    {
        return msg.GetMessageHead().rKey.empty();
    }

    MessageBrokerImpl::listenersList_t MessageBrokerImpl::GetListenersForQueue(const queueName_t &queueName) const
    {
        return queueNameToMessageListener.at(queueName);
    }
} // !namespace Ensety::MessageBroker
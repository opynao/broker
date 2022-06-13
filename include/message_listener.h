#ifndef __MESSAGE_LISTENER_H__
#define __MESSAGE_LISTENER_H__

namespace Ensety::MessageBroker
{
    template <typename MessageType>
    class IMessageListener
    {
    public:
        virtual ~IMessageListener() = default;
        virtual void OnMessageReceived(const MessageType &) = 0;
    };
} // !namespace Ensety::MessageBroker

#endif
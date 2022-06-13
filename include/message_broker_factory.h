#ifndef __MESSAGE_BROKER_FACTORY_H__
#define __MESSAGE_BROKER_FACTORY_H__

#include <memory>

namespace Ensety::MessageBroker
{
    class IMessageBrokerChannel;
}// !namespace Ensety::MessageBroker

namespace Ensety::MessageBroker::Factory
{
    std::unique_ptr<IMessageBrokerChannel> CreateChannel();
} // !namespace Ensety::MessageBroker::Factory
#endif // !__MESSAGE_BROKER_FACTORY_H__
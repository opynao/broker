#ifndef __QUEUE_PARAMS_H__
#define __QUEUE_PARAMS_H__

#include <string>

namespace Ensety::MessageBroker
{
    using queueName_t = std::string;
    using routingKey_t = std::string;

    struct QueueParams
    {
        queueName_t queueName;
        routingKey_t routingKey;
    };
} // !namespace Ensety::MessageBroker
#endif
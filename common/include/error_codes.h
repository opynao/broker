#ifndef __ERROR_CODES_H__
#define __ERROR_CODES_H__

namespace Ensety
{
    enum class ErrorCodes
    {
        Success = 0,
        MessageBroker_Error = 5000,
            MessageBroker_QueueUnknownError = 5200,
                MessageBroker_QueueNotExistError = 5201,
                MessageBroker_QueueAlreadyExists = 5202,
                MessageBroker_EmptyRoutingKey = 5221
    };
} // !namespace Ensety

#endif
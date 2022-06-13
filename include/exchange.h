#ifndef __EXCHANGE_H__
#define __EXCHANGE_H__

namespace Ensety::MessageBroker
{
    enum class ExchangeType
    {
        Direct,
        Fanout,
        Topic,
        Headers
    };

    struct ExchangeParams
    {
        std::string name;
    };

    class Exchange
    {
    public:
    private:
    };
} // !namespace Ensety::MessageBroker
#endif

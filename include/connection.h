// class IConnection
// {
// public:
//     virtual std::unique_ptr<Channel> CreateChannel() const = 0;
//     virtual ~IConnection() {}
// };

// class Connection : public IConnection
// {
// public:
//     virtual std::unique_ptr<Channel> CreateChannel() const override
//     {
//         return std::make_unique<Channel>();
//     }
// };

// class ConnectionFactory
// {
// public:
//     virtual std::unique_ptr<Connection> CreateConnection() const = 0;
//     virtual ~ConnectionFactory() {}
// };

// class MQConnectionFactory : public ConnectionFactory
// {
// public:
//     std::unique_ptr<Connection> CreateConnection() const override
//     {
//         return std::make_unique<Connection>();
//     }
// };

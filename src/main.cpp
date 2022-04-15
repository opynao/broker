#include "queue.h"
#include "message_broker.h"
#include "message.h"
#include "logger.h"
#include "utils.h"

#include <memory>

int main()
{
    auto &broker{MessageBroker::GetInstance()};

    auto logger = std::make_shared<Logger>();
    // Register Logger queue so that logger can get msgs
    const std::string loggerQueueName{"logger_queue"};
    broker.QueueRegister(loggerQueueName, "logger.queue", logger);

    auto gui = std::make_shared<GUI>();

    const std::string loggerRoutingKey{"logger.queue"};
    const std::string guiRoutingKey{"gui.queue"};

    // Send msg to logger from gui through routing key
    auto msg1 = MessageFactoryType::CreateMessage({loggerRoutingKey, guiRoutingKey}, "gui sends msg to logger");
    broker.Publish(msg1); // << gui sends msg to logger

    auto msg2 = MessageFactoryType::CreateMessage({guiRoutingKey}, "msg to gui");
    broker.Publish(msg2); // << No queue with rKey gui.queue

    // Register GUI queue so that it can get msgs
    broker.QueueRegister("gui_queue", guiRoutingKey, gui);
    broker.Publish(msg2); //  << msg to gui

    // Send msg to logger queue from gui through queueName
    auto msg3 = MessageFactoryType::CreateMessage({"", guiRoutingKey}, "gui sends msg to logger");
    broker.Publish(msg3, loggerQueueName);
}

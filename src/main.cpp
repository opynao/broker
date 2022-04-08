#include "queue.h"
#include "message_broker.h"
#include "message.h"
#include "utils.h"

#include <memory>

int main()
{
    MessageBroker broker;
    broker.QueueRegister("logger_queue", "logger.queue");
    
    auto logger = std::make_shared<Logger>();
    broker.ObserverRegister("logger_queue", logger);
    
    auto gui = std::make_shared<GUI>();
    broker.ObserverRegister("gui_queue", logger);

    auto msg1 = MessageFactoryType::CreateMessage({"logger.queue", "gui.queue"}, "gui sends to logger");
    broker.Publish(msg1);
    
    auto msg2 = MessageFactoryType::CreateMessage({"gui.queue"}, "msg to gui");
    broker.Publish(msg2);
}

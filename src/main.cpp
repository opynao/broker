#include "queue.h"
#include "message_broker.h"
#include "message.h"
#include "utils.h"

#include <memory>

int main()
{
    auto broker = MessageBroker::GetInstance();

    auto logger = std::make_shared<Logger>();
    // Register Logger queue so that logger can get msgs
    broker->QueueRegister("logger_queue", "logger.queue", logger);

    auto gui = std::make_shared<GUI>();

    // Create msg to logger from gui
    auto msg1 = MessageFactoryType::CreateMessage({"logger.queue", "gui.queue"}, "gui sends to logger");
    // Send msg to logger queue
    broker->Publish(msg1); // gui sends to logger

    auto msg2 = MessageFactoryType::CreateMessage({"gui.queue"}, "msg to gui");
    broker->Publish(msg2); // No queue with rKey gui.queue

    broker->QueueRegister("gui_queue", "gui.queue", gui);
    broker->Publish(msg2); // msg to gui
}

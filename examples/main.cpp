#include "libcpp-event-hub.hpp"
#include <string>
#include <iostream>

using namespace lklibs;

int main()
{
    // Get the singleton instance of the EventHub
    auto& eventHub = EventHub::getInstance();

    // Add a listener for testEvent1, which sends a string value
    auto listener1Id = eventHub.addListener<std::string>("testEvent1", [](const std::string& eventName, const std::string& sender, const std::string& data)
    {
        std::cout << "1. listener received, Event name: " << eventName << ", sender: " << sender << ", data: " << data << std::endl;
    });

    // Add a listener for testEvent2, which sends an int value
    auto listener2Id = eventHub.addListener<int>("testEvent2", [](const std::string& eventName, const std::string& sender, const int& data)
    {
        std::cout << "2 listener received, Event name: " << eventName << ", sender: " << sender << ", data: " << data << std::endl;
    });

    // Add a general listener for all events with a string value
    auto listener3Id = eventHub.addListener<std::string>("*", [](const std::string& eventName, const std::string& sender, const std::string& data)
    {
        std::cout << "General listener received: " << eventName << ", sender: " << sender << ", data: " << data << std::endl;
    });

    // Emit testEvent1 with a string value, 2 listeners will receive the event (listener1 and the general listener)
    eventHub.emit("testEvent1", "main", std::string("Value 1"));

    // Emit testEvent2 with an int value, 1 listeners will receive the event (listener2), because there is no general listener for int values
    eventHub.emit("testEvent2", "main", 7);

    // Remove the first listener for testEvent1
    eventHub.removeListener("testEvent1", listener1Id);

    // Emit testEvent1 again, only the general listener will receive the event
    eventHub.emit("testEvent1", "main", std::string("Value 2"));

    return 0;
}

#include <gtest/gtest.h>
#include "libcpp-event-hub.hpp"
#include <string>
#include <thread>

using namespace lklibs;

TEST(EventHubTest, AddListenerAndEmitString)
{
    auto& eventHub = EventHub::getInstance();

    bool listenerCalled = false;

    auto listenerId = eventHub.addListener<std::string>("testEvent", [&](const std::string& eventName, const std::string& sender, const std::string& data)
    {
        listenerCalled = true;

        EXPECT_EQ(eventName, "testEvent");
        EXPECT_EQ(sender, "testSender");
        EXPECT_EQ(data, "testData");
    });

    eventHub.emit("testEvent", "testSender", std::string("testData"));

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_TRUE(listenerCalled);

    eventHub.removeListener("testEvent", listenerId);
}

TEST(EventHubTest, AddListenerAndEmitInt)
{
    auto& eventHub = EventHub::getInstance();

    bool listenerCalled = false;

    auto listenerId = eventHub.addListener<int>("testEvent", [&](const std::string& eventName, const std::string& sender, const int& data)
    {
        listenerCalled = true;

        EXPECT_EQ(eventName, "testEvent");
        EXPECT_EQ(sender, "testSender");
        EXPECT_EQ(data, 7);
    });

    eventHub.emit("testEvent", "testSender", 7);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_TRUE(listenerCalled);

    eventHub.removeListener("testEvent", listenerId);
}

TEST(EventHubTest, GeneralListener)
{
    auto& eventHub = EventHub::getInstance();

    bool generalListenerCalled = false;
    bool specificListenerCalled = false;

    auto generalListenerId = eventHub.addListener<std::string>("*", [&](const std::string& eventName, const std::string& sender, const std::string& data)
    {
        generalListenerCalled = true;

        EXPECT_EQ(eventName, "testEvent");
        EXPECT_EQ(sender, "testSender");
        EXPECT_EQ(data, "testData");
    });

    auto specificListenerId = eventHub.addListener<std::string>("testEvent", [&](const std::string& eventName, const std::string& sender, const std::string& data)
    {
        specificListenerCalled = true;

        EXPECT_EQ(eventName, "testEvent");
        EXPECT_EQ(sender, "testSender");
        EXPECT_EQ(data, "testData");
    });

    eventHub.emit("testEvent", "testSender", std::string("testData"));

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_TRUE(generalListenerCalled);
    EXPECT_TRUE(specificListenerCalled);

    eventHub.removeListener("*", generalListenerId);
    eventHub.removeListener("testEvent", specificListenerId);
}

TEST(EventHubTest, RemoveListener)
{
    auto& eventHub = EventHub::getInstance();

    bool listenerCalled = false;

    auto listenerId = eventHub.addListener<std::string>("testEvent", [&](const std::string& eventName, const std::string& sender, const std::string& data)
    {
        listenerCalled = true;
    });

    eventHub.removeListener("testEvent", listenerId);

    eventHub.emit("testEvent", "testSender", std::string("testData"));

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_FALSE(listenerCalled);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

/*

Thread-safe generic event library for C++ (17+)
version 1.0.0
https://github.com/leventkaragol/libcpp-event-hub

If you encounter any issues, please submit a ticket at https://github.com/leventkaragol/libcpp-event-hub/issues

Copyright (c) 2024 Levent KARAGÖL

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef LIBCPP_EVENT_HUB_HPP
#define LIBCPP_EVENT_HUB_HPP

#include <string>
#include <unordered_map>
#include <functional>
#include <shared_mutex>
#include <any>
#include <iostream>
#include <mutex>

namespace lklibs
{
    /**
     * @brief Thread-safe generic Event hub class
     */
    class EventHub
    {
    public:
        using ListenerFunction = std::function<void(const std::string&, const std::string&, const std::any&)>;

        using ListenerId = size_t;

        /**
         * @brief Get the singleton instance of the EventHub
         *
         * @return EventHub&
         */
        static EventHub& getInstance()
        {
            static EventHub instance;
            return instance;
        }

        /**
         * @brief Emit an event with generic type
         *
         * @tparam EventData: Type of the data to be sent with the event
         * @param eventName: Name of the event
         * @param sender: Sender of the event
         * @param data: Data to be sent with the event
         */
        template <typename EventData>
        void emit(const std::string& eventName, const std::string& sender, const EventData& data)
        {
            std::shared_lock lock(mutex_);

            std::any eventData = data;

            if (listeners_.find(eventName) != listeners_.end())
            {
                for (const auto& [id, listener] : listeners_.at(eventName))
                {
                    listener(eventName, sender, eventData);
                }
            }

            if (listeners_.find("*") != listeners_.end())
            {
                for (const auto& [id, listener] : listeners_.at("*"))
                {
                    listener(eventName, sender, eventData);
                }
            }
        }

        /**
         * @brief Add a listener for an event
         *
         * @tparam EventData: Type of the data to be sent with the event
         * @param eventName: Name of the event
         * @param listener: Listener function to be called when the event is emitted
         * @return Listener Id
         */
        template <typename EventData>
        ListenerId addListener(const std::string& eventName, std::function<void(const std::string&, const std::string&, const EventData&)> listener)
        {
            std::unique_lock lock(mutex_);

            auto id = nextListenerId_++;

            auto wrappedListener = [listener](const std::string& eventName, const std::string& sender, const std::any& data)
            {
                if (data.type() == typeid(EventData))
                {
                    listener(eventName, sender, std::any_cast<const EventData&>(data));
                }
            };

            listeners_[eventName][id] = wrappedListener;

            return id;
        }

        /**
         * @brief Remove a listener for an event
         *
         * @param eventName: Name of the event
         * @param listenerId: Id of the listener to be removed
         */
        void removeListener(const std::string& eventName, ListenerId listenerId)
        {
            std::unique_lock lock(mutex_);

            if (listeners_.find(eventName) != listeners_.end())
            {
                listeners_[eventName].erase(listenerId);

                if (listeners_[eventName].empty())
                {
                    listeners_.erase(eventName);
                }
            }
        }

    private:
        EventHub() = default;
        EventHub(const EventHub&) = delete;
        EventHub& operator=(const EventHub&) = delete;

        std::unordered_map<std::string, std::unordered_map<ListenerId, ListenerFunction>> listeners_;
        mutable std::shared_mutex mutex_;
        ListenerId nextListenerId_ = 0;
    };
}

#endif // LIBCPP_EVENT_HUB_HPP

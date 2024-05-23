# libcpp-event-hub

Thread-safe generic event library for C++ (17+)

[![linux](https://github.com/leventkaragol/libcpp-event-hub/actions/workflows/linux.yml/badge.svg)](https://github.com/leventkaragol/libcpp-event-hub/actions/workflows/linux.yml)
[![windows](https://github.com/leventkaragol/libcpp-event-hub/actions/workflows/windows.yml/badge.svg)](https://github.com/leventkaragol/libcpp-event-hub/actions/workflows/windows.yml)

> [!TIP]
> Please read this document before using the library. I know, you don't have time but reading
> this document will save you time. I mean just this file, it's not long at all. Trial and error
> will cost you more time.

# Table of Contents

* [What is this library used for?](#what-is-this-library-used-for)
* [How to add it to my project](#how-to-add-it-to-my-project)
* [How to use?](#how-to-use)
* [Semantic Versioning](#semantic-versioning)
* [License](#license)
* [Contact](#contact)

## What is this library used for?

C++ standard library does not include a ready to use Event Handling mechanism. This library is designed to work as a 
singleton throughout the application and to communicate thread-safely between different code blocks via the required 
events with its generic structure.

With the Event Hub library, you can add or remove new listeners to the events, as you are used to in other languages, 
listen to a single event or all events from one place and emit them with the required type of data.


## How to add it to my project?

This is a header only library with no external dependency. So actually, all you need is to add the libcpp-event-hub.hpp file
in src folder to your project and start using it with #include.

You can find usage examples in the examples folder, also find a sample CMakeLists.txt file content below.

```cmake
cmake_minimum_required(VERSION 3.14)

project(myProject)

add_executable(myProject main.cpp)

target_link_libraries(myProject PRIVATE libcpp-event-hub)

```

## How to use?

You can find a simple example of adding & deleting event listeners and emitting events to handle different types of them below.

```cpp
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
```

## Semantic Versioning

Versioning of the library is done using conventional semantic versioning. Accordingly,
in the versioning made as **MAJOR.MINOR.PATCH**;

**PATCH:** Includes possible Bug&Fixes and improvements. You definitely want to get this.

**MINOR:** Additional functionality added via backwards compatibility. You probably want to
get this, it doesn't hurt.

**MAJOR:** Additional functionality that breaks backwards compatibility. You'll need to know
what's changed before you get it, and you'll probably have to make changes to your own code.
If I publish something like this, I will definitely add the changes required for migration
section to the documentation.

## License

MIT License

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

## Contact

If you have problems regarding the library, please open an
[issue on GitHub](https://github.com/leventkaragol/libcpp-event-hub/issues/new).
Please describe your request, issue, or question in as much detail as possible
and also include the version of your compiler and operating system, as well as
the version of the library you are using. Before opening a new issue, please
confirm that the topic is not already exists in closed issues.
cmake_minimum_required(VERSION 3.14)

project(examples)

add_executable(examples main.cpp)

target_link_libraries(examples PRIVATE libcpp-event-hub)

cmake_minimum_required(VERSION 4.0)
project(HTTPServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(HTTPServer main.cpp
        Order.h
        Portfolio.cpp
        Portfolio.h
        ../SignalReceiver.cpp
        SignalReceiver.h
        tests.cpp
        tests.h)

target_link_libraries(HTTPServer ws2_32)
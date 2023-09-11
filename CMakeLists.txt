cmake_minimum_required(VERSION 3.26)
project(Network)

set(CMAKE_CXX_STANDARD 14)

add_executable(Network Server.cpp
        Client.cpp)

target_link_libraries(Network wsock32 ws2_32)

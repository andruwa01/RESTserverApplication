cmake_minimum_required(VERSION 3.10.0)
project(clientAndServer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system filesystem)

add_executable(serverApp src/server.cpp)
add_executable(clientApp src/client.cpp)
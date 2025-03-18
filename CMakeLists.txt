cmake_minimum_required(VERSION 3.10.0)
project(clientAndServer VERSION 0.1.0 LANGUAGES CXX)

add_executable(serverApp src/server.cpp)
add_executable(clientApp src/client.cpp)
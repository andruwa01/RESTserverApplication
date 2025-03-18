cmake_minimum_required(VERSION 3.10.0)
project(clientAndServer VERSION 0.1.0 LANGUAGES C CXX)

add_executable(serverApp server.cpp)
add_executable(clientApp client.cpp)
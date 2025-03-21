cmake_minimum_required(VERSION 3.10.0)
project(clientAndServer VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/headers)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

set(SOURCE_FILES
    src/server.cpp
    src/handle_request.cpp
    src/session.cpp
    src/listener.cpp
    src/database.cpp
)

# find_package(Boost REQUIRED COMPONENTS system filesystem)
add_executable(serverApp ${SOURCE_FILES})

add_subdirectory(deps/libpqxx build-pqxx)
target_link_libraries(serverApp PRIVATE pqxx)
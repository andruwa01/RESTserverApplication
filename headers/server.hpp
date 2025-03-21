#ifndef SERVER_H_
#define SERVER_H_

#include "listener.hpp"
#include "database.hpp"
#include <memory>

extern std::unique_ptr<Database> dbconnection;
extern net::io_context ioc;

#endif
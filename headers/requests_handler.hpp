#ifndef HANDLE_REQUEST_H_
#define HANDLE_REQUEST_H_

#include "boost_namespaces.hpp"
#include "database.hpp"
#include "server.hpp"
#include <nlohmann/json.hpp>
#include <string>

http::response<http::string_body> handle_request(const http::request<http::string_body>& req);

#endif
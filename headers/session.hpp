#ifndef SESSTION_H_
#define SESSTION_H_

#include "boost_namespaces.hpp"
#include "requests_handler.hpp"
#include <memory>
#include <iostream>

class Session : public std::enable_shared_from_this<Session> {
    tcp::socket socket_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;

public:
    explicit Session(tcp::socket socket);
    ~Session();

    void run();

private:
    void do_read();
    void do_write(http::response<http::string_body> res);
};

#endif
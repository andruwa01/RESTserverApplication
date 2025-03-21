// Listener.h
#ifndef LISTENER_H_
#define LISTENER_H_

#include "boost_namespaces.h"
#include "session.h"
#include <memory>

class Listener : public std::enable_shared_from_this<Listener>
{
    net::io_context& ioc_;
    tcp::acceptor acceptor_;

public:
    Listener(net::io_context& ioc, tcp::endpoint endpoint);
    void run();

private:
    void do_accept();
};

#endif
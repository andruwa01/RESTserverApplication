// Listener.cpp
#include "listener.hpp"
#include "requests_handler.hpp"
#include <iostream>

Listener::Listener(net::io_context& ioc, tcp::endpoint endpoint) : ioc_(ioc), acceptor_(net::make_strand(ioc))
{
    beast::error_code ec;

    // Open the acceptor
    acceptor_.open(endpoint.protocol(), ec);
    if (ec)
    {
        std::cerr << "Open error: " << ec.message() << std::endl;
        return;
    }

    // Allow address reuse
    acceptor_.set_option(net::socket_base::reuse_address(true), ec);
    if (ec)
    {
        std::cerr << "Set option error: " << ec.message() << std::endl;
        return;
    }

    // Bind to the server address
    acceptor_.bind(endpoint, ec);
    if (ec)
    {
        std::cerr << "Bind error: " << ec.message() << std::endl;
        return;
    }

    // Start listening for connections
    acceptor_.listen(net::socket_base::max_listen_connections, ec);
    if (ec)
    {
        std::cerr << "Listen error: " << ec.message() << std::endl;
        return;
    }

    std::cout << "started listening new connections\n";
}

void Listener::run()
{
    do_accept();
}

void Listener::do_accept()
{
    acceptor_.async_accept(net::make_strand(ioc_), [self = weak_from_this()](beast::error_code ec, tcp::socket socket)
    {
        if (auto shared_self = self.lock())
        {
            if (!ec)
            {
                auto session = std::make_shared<Session>(std::move(socket));
                session->run();
            }

            shared_self->do_accept();
        }
    });
}
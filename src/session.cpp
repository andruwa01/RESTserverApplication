#include "session.hpp"

Session::Session(tcp::socket socket) : socket_(std::move(socket))
{
    std::cout << "session started\n";
}

Session::~Session()
{
    std::cout << "session closed\n";
}

void Session::run()
{
    do_read();
}

void Session::do_read()
{
    auto self(shared_from_this());
    http::async_read(socket_, buffer_, req_, [this, self](beast::error_code ec, std::size_t)
    {
        std::cout << "receive request\n";

        if (!ec)
        {
            do_write(handle_request(req_));
        }
    });
}

void Session::do_write(http::response<http::string_body> res)
{
    std::cout << "send response\n";

    auto self(shared_from_this());
    auto sp = std::make_shared<http::response<http::string_body>>(std::move(res));
    http::async_write(socket_, *sp, [this, self, sp](beast::error_code ec, std::size_t)
    {
        socket_.shutdown(tcp::socket::shutdown_send, ec);
    });
}
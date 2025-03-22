#ifndef NAMESPACES_H_
#define NAMESPACES_H_

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
// #include <boost/beast/version.hpp>
// #include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
// #include <boost/config.hpp>

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

#endif
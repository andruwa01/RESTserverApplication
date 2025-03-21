#include "server.hpp"

std::unique_ptr<Database> dbconnection;

int main() {
    try
    {
        dbconnection = std::make_unique<Database>("dbname=tasks_employees user=user password=123 host=localhost port=5432");

        const auto address = net::ip::make_address("127.0.0.1"); // use 0.0.0.0 for all interfaces on machine
        unsigned short port = 8080;

        net::io_context ioc{1};

        auto listener = std::make_shared<Listener>(ioc, tcp::endpoint{address, port});
        listener->run();

        ioc.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
#include "database.hpp"

Database::Database(const std::string& connection_string)
    : conn_(connection_string)
{
    std::cout << "connected to db\n";
}

Database::~Database()
{
    std::cout << "disconnected from db\n";
}

void Database::testConnection()
{
    try
    {
        pqxx::work tx(conn_);
        pqxx::row r = tx.exec1("SELECT 1");
        tx.commit();
        std::cout << r[0].as<int>() << std::endl;
    }
    catch(std::exception const &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

pqxx::connection& Database::getConnection()
{
    return conn_;
}
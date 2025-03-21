#include "database.hpp"

Database::Database(const std::string& connection_string)
    : conn_(connection_string)
{
    std::cout << "connected to db\n";
}

Database::~Database()
{
}

void Database::get_one()
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

std::string Database::get_all_employees()
{
    pqxx::work txn(conn_);
    pqxx::result result = txn.exec("SELECT * FROM employees");

    if (result.empty())
    {
        return "{}"; // Пустой ответ, если данных нет
    }

    return result[0][0].as<std::string>();
}
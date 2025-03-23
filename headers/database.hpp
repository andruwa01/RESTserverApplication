// DatabaseConnection.h
#ifndef DATABASE_H_
#define DATABASE_H_

#include "boost_namespaces.hpp"

#include <pqxx/pqxx>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>

class DatabaseConnection
{
public:
    DatabaseConnection(const std::string& connection_string);
    ~DatabaseConnection();

    void testConnection();
    pqxx::connection& getConnection();

    nlohmann::json getAllEmployees();
    nlohmann::json createEmployee(const nlohmann::json& employee_data);
    nlohmann::json getEmployeeById(int id);

private:
    pqxx::connection conn_;
};

#endif
// DatabaseConnection.h
#ifndef DATABASE_H_
#define DATABASE_H_

#include "boost_namespaces.hpp"

#include <pqxx/pqxx>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>

class DatabaseConnection
{
public:
    DatabaseConnection(const std::string& connection_string);
    ~DatabaseConnection();

    void testConnection();

    nlohmann::json getAllEmployees();
    nlohmann::json createEmployee(const nlohmann::json& employee_data);
    nlohmann::json getEmployeeById(int id);
    nlohmann::json deleteEmployeeById(int id);
    nlohmann::json updateEmployeeById(int id, const nlohmann::json& employee_data);

    nlohmann::json getAllTasks();
    nlohmann::json getTaskById(int id);
    nlohmann::json createTask(const nlohmann::json& task_data);
    nlohmann::json updateTaskById(int id, const nlohmann::json& task_data);
    nlohmann::json deleteTaskById(int id);

private:
    pqxx::connection conn_;
};

#endif
#include "database.hpp"

DatabaseConnection::DatabaseConnection(const std::string& connection_string)
    : conn_(connection_string)
{
    std::cout << "connected to db\n";
}

DatabaseConnection::~DatabaseConnection()
{
    std::cout << "disconnected from db\n";
}

void DatabaseConnection::testConnection()
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

pqxx::connection& DatabaseConnection::getConnection()
{
    return conn_;
}

nlohmann::json DatabaseConnection::getAllEmployees()
{
    pqxx::work txn(conn_);
    pqxx::result res = txn.exec("SELECT * FROM employees");
    nlohmann::json employees = nlohmann::json::array();
    for (const auto& row : res) {
        employees.push_back({
            {"id", row["id"].as<int>()},
            {"full_name", row["full_name"].as<std::string>()},
            {"position", row["position"].as<std::string>()}
        });
    }
    txn.commit();
    return employees; 
}

nlohmann::json DatabaseConnection::createEmployee(const nlohmann::json& employee_data)
{
    pqxx::work txn(conn_);
    pqxx::result res = txn.exec_params
    (
        "INSERT INTO employees (full_name, position) VALUES ($1, $2) RETURNING *",
        employee_data["full_name"].get<std::string>(), employee_data["position"].get<std::string>()
    );
    nlohmann::json new_employee =
    {
        {"id", res[0]["id"].as<int>()},
        {"full_name", res[0]["full_name"].as<std::string>()},
        {"position", res[0]["position"].as<std::string>()}
    };

    txn.commit();
    return new_employee;
}

nlohmann::json DatabaseConnection::getEmployeeById(int id)
{
    pqxx::work txn(conn_);
    pqxx::result res = txn.exec_params("SELECT * FROM employees WHERE id = $1", id);
    nlohmann::json employee;
    if (!res.empty()) {
        employee = {
            {"id", res[0]["id"].as<int>()},
            {"full_name", res[0]["full_name"].as<std::string>()},
            {"position", res[0]["position"].as<std::string>()}
        };
    }
    txn.commit();
    return employee;
}

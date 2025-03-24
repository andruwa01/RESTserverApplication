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

nlohmann::json DatabaseConnection::deleteEmployeeById(int id)
{
    pqxx::work txn(conn_);
    pqxx::result res = txn.exec_params("DELETE FROM employees WHERE id = $1 RETURNING *", id);
    nlohmann::json deleted_employee;
    if (!res.empty()) {
        deleted_employee = {
            {"id", res[0]["id"].as<int>()},
            {"full_name", res[0]["full_name"].as<std::string>()},
            {"position", res[0]["position"].as<std::string>()}
        };
    }
    txn.commit();
    return deleted_employee;
}

nlohmann::json DatabaseConnection::updateEmployeeById(int id, const nlohmann::json& employee_data)
{
    pqxx::work txn(conn_);
    pqxx::result res = txn.exec_params(
        "UPDATE employees SET full_name = $1, position = $2 WHERE id = $3 RETURNING *", 
        employee_data["full_name"].get<std::string>(), 
        employee_data["position"].get<std::string>(), 
        id
    );
    nlohmann::json updated_employee;
    if (!res.empty()) {
        updated_employee = {
            {"id", res[0]["id"].as<int>()},
            {"full_name", res[0]["full_name"].as<std::string>()},
            {"position", res[0]["position"].as<std::string>()}
        };
    }
    txn.commit();
    return updated_employee;
}

nlohmann::json DatabaseConnection::getAllTasks()
{
    pqxx::work txn(conn_);
    pqxx::result res = txn.exec("SELECT * FROM tasks");
    nlohmann::json tasks = nlohmann::json::array();
    for (const auto& row : res) {
        tasks.push_back({
            {"id", row["id"].as<int>()},
            {"title", row["title"].as<std::string>()},
            {"description", row["description"].as<std::string>()},
            {"parent_task_id", res[0]["parent_task_id"].is_null() ? nlohmann::json(nullptr) : nlohmann::json(res[0]["parent_task_id"].as<int>())},
            {"assignee_id",    res[0]["assignee_id"].is_null()    ? nlohmann::json(nullptr) : nlohmann::json(res[0]["assignee_id"].as<int>())},
            {"due_date", row["due_date"].as<std::string>()},
            {"status", row["status"].as<std::string>()}
        });
    }
    txn.commit();
    return tasks;
}

nlohmann::json DatabaseConnection::getTaskById(int id)
{
    pqxx::work txn(conn_);
    pqxx::result res = txn.exec_params("SELECT * FROM tasks WHERE id = $1", id);
    nlohmann::json task;
    if (!res.empty()) {
        task = {
            {"id", res[0]["id"].as<int>()},
            {"title", res[0]["title"].as<std::string>()},
            {"description", res[0]["description"].as<std::string>()},
            {"parent_task_id", res[0]["parent_task_id"].is_null() ? nlohmann::json(nullptr) : nlohmann::json(res[0]["parent_task_id"].as<int>())},
            {"assignee_id",    res[0]["assignee_id"].is_null()    ? nlohmann::json(nullptr) : nlohmann::json(res[0]["assignee_id"].as<int>())},
            {"due_date", res[0]["due_date"].as<std::string>()},
            {"status", res[0]["status"].as<std::string>()}
        };
    }
    txn.commit();
    return task;
}

nlohmann::json DatabaseConnection::createTask(const nlohmann::json& task_data)
{
    pqxx::work txn(conn_);

    std::optional<int> parent_task = (task_data.contains("parent_task_id") && !task_data["parent_task_id"].is_null())
                           ? std::make_optional(task_data["parent_task_id"].get<int>())
                           : std::nullopt;
    std::optional<int> assignee = (task_data.contains("assignee_id") && !task_data["assignee_id"].is_null())
                           ? std::make_optional(task_data["assignee_id"].get<int>())
                           : std::nullopt;

    // checking parent_task exists in db or not
    if (parent_task)
    {
        pqxx::result parent_check = txn.exec_params("SELECT id FROM tasks WHERE id = $1", parent_task.value());
        if (parent_check.empty())
        {
            txn.abort();
            return {{"error", "Parent task with given id does not exist"}};
        }
    }

    /// checking assignee exists in db or not
    if (assignee)
    {
        pqxx::result assignee_check = txn.exec_params("SELECT id FROM employees WHERE id = $1", assignee.value());
        if (assignee_check.empty())
        {
            txn.abort();
            return {{"error", "Assignee with given id does not exist"}};
        }
    }

    pqxx::result res = txn.exec_params(
        "INSERT INTO tasks (title, description, parent_task_id, assignee_id, due_date, status) "
        "VALUES ($1, $2, $3, $4, $5, $6) RETURNING *",
        task_data["title"].get<std::string>(),
        task_data["description"].get<std::string>(),
        parent_task,
        assignee,
        task_data["due_date"].get<std::string>(),
        task_data["status"].get<std::string>()
    );

    nlohmann::json new_task = {
        {"id", res[0]["id"].as<int>()},
        {"title", res[0]["title"].as<std::string>()},
        {"description", res[0]["description"].as<std::string>()},
        {"parent_task_id", res[0]["parent_task_id"].is_null() ? nlohmann::json(nullptr) : nlohmann::json(res[0]["parent_task_id"].as<int>())},
        {"assignee_id",    res[0]["assignee_id"].is_null()    ? nlohmann::json(nullptr) : nlohmann::json(res[0]["assignee_id"].as<int>())},
        {"due_date", res[0]["due_date"].as<std::string>()},
        {"status", res[0]["status"].as<std::string>()}
    };

    txn.commit();
    return new_task;
}

nlohmann::json DatabaseConnection::updateTaskById(int id, const nlohmann::json& task_data)
{
    pqxx::work txn(conn_);

    std::optional<int> parent_task = (task_data.contains("parent_task_id") && !task_data["parent_task_id"].is_null())
                           ? std::make_optional(task_data["parent_task_id"].get<int>())
                           : std::nullopt;
    std::optional<int> assignee = (task_data.contains("assignee_id") && !task_data["assignee_id"].is_null())
                           ? std::make_optional(task_data["assignee_id"].get<int>())
                           : std::nullopt;

    pqxx::result res = txn.exec_params(
        "UPDATE tasks SET title = $1, description = $2, parent_task_id = $3, assignee_id = $4, due_date = $5, status = $6 "
        "WHERE id = $7 RETURNING *",
        task_data["title"].get<std::string>(),
        task_data["description"].get<std::string>(),
        parent_task,
        assignee,
        task_data["due_date"].get<std::string>(),
        task_data["status"].get<std::string>(),
        id
    );

    nlohmann::json updated_task;
    if (!res.empty()) {
        updated_task = {
            {"id", res[0]["id"].as<int>()},
            {"title", res[0]["title"].as<std::string>()},
            {"description", res[0]["description"].as<std::string>()},
            {"parent_task_id", res[0]["parent_task_id"].is_null() ? nlohmann::json(nullptr) : nlohmann::json(res[0]["parent_task_id"].as<int>())},
            {"assignee_id",    res[0]["assignee_id"].is_null()    ? nlohmann::json(nullptr) : nlohmann::json(res[0]["assignee_id"].as<int>())},
            {"due_date", res[0]["due_date"].as<std::string>()},
            {"status", res[0]["status"].as<std::string>()}
        };
    }
    txn.commit();
    return updated_task;
}

nlohmann::json DatabaseConnection::deleteTaskById(int id)
{
    pqxx::work txn(conn_);
    pqxx::result res = txn.exec_params("DELETE FROM tasks WHERE id = $1 RETURNING *", id);
    nlohmann::json deleted_task;
    if (!res.empty()) {
        deleted_task = {
            {"id", res[0]["id"].as<int>()},
            {"title", res[0]["title"].as<std::string>()},
            {"description", res[0]["description"].as<std::string>()},
            {"parent_task_id", res[0]["parent_task_id"].is_null() ? nlohmann::json(nullptr) : nlohmann::json(res[0]["parent_task_id"].as<int>())},
            {"assignee_id", res[0]["assignee_id"].is_null()       ? nlohmann::json(nullptr) : nlohmann::json(res[0]["assignee_id"].as<int>())},
            {"due_date", res[0]["due_date"].as<std::string>()},
            {"status", res[0]["status"].as<std::string>()}
        };
    }
    txn.commit();
    return deleted_task;
}
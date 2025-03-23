#include "requests_handler.hpp"

http::response<http::string_body> handle_request(const http::request<http::string_body>& req)
{
    DatabaseConnection db("dbname=tasks_employees user=user password=123 host=localhost port=5432");

    if (req.method() == http::verb::get && req.target() == "/api/data")
    {
        db.testConnection();

        // handle GET request
        nlohmann::json json_response = {{"message", "this is a GET request"}}; 

        std::string json_str = R"({"key": "value"})";
        nlohmann::json json_addition = nlohmann::json::parse(json_str);

        json_response.update(json_addition);

        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        res.body() = json_response.dump();
        res.prepare_payload();
        return res;
    }

    // <------------------ handling employees table ------------------>
    else if (req.method() == http::verb::get && req.target() == "/api/employees")
    {
        // get all employees
        nlohmann::json employees = db.getAllEmployees();

        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::content_type, "application/json");
        res.body() = employees.dump();
        res.prepare_payload();
        return res;
    }
    else if (req.method() == http::verb::get && req.target().starts_with("/api/employees"))
    {
        // get employee by id
        size_t base_size = std::string("/api/employees").length();
        int id = std::stoi(std::string(req.target()).substr(base_size + 1));
        nlohmann::json employee = db.getEmployeeById(id);

        http::response<http::string_body> res{employee.empty() ? http::status::not_found : http::status::ok, req.version()};
        res.set(http::field::content_type, "application/json");
        res.body() = employee.empty() ? "{\"error\": \"Employee not found\"}" : employee.dump();
        res.prepare_payload();
        return res;
    }
    else if (req.method() == http::verb::post && req.target() == "/api/employees")
    {
        // create new employee
        nlohmann::json request_body = nlohmann::json::parse(req.body());
        nlohmann::json new_employee = db.createEmployee(request_body);

        http::response<http::string_body> res{http::status::created, req.version()};
        res.set(http::field::content_type, "application/json");
        res.body() = new_employee.dump();
        res.prepare_payload();
        return res;
    }
    else if (req.method() == http::verb::put && req.target().starts_with("/api/employees"))
    {
        // update employee by id
        size_t base_size = std::string("/api/employees").length();
        int id = std::stoi(std::string(req.target()).substr(base_size + 1));
        nlohmann::json request_body = nlohmann::json::parse(req.body());
        nlohmann::json updated_employee = db.updateEmployeeById(id, request_body);

        http::response<http::string_body> res{updated_employee.empty() ? http::status::not_found : http::status::ok, req.version()};
        res.set(http::field::content_type, "application/json");
        res.body() = updated_employee.empty() ? "{\"error\": \"Employee not found\"}" : updated_employee.dump();
        res.prepare_payload();
        return res;
    }
    else if (req.method() == http::verb::delete_ && req.target().starts_with("/api/employees/"))
    {
        // delete employee by id
        size_t base_size = std::string("/api/employees").length();
        int id = std::stoi(std::string(req.target()).substr(base_size + 1));
        nlohmann::json deleted_employee = db.deleteEmployeeById(id);

        http::response<http::string_body> res{deleted_employee.empty() ? http::status::not_found : http::status::ok, req.version()};
        res.set(http::field::content_type, "application/json");
        res.body() = deleted_employee.empty() ? "{\"error\": \"Employee not found\"}" : deleted_employee.dump();
        res.prepare_payload();
        return res;
    }

    // <------------------ handling tasks table ------------------>
    else if (req.method() == http::verb::get && req.target() == "/api/tasks")
    {
        // get all tasks
        nlohmann::json tasks = db.getAllTasks();
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::content_type, "application/json");
        res.body() = tasks.dump();
        res.prepare_payload();
        return res;
    }
    else if (req.method() == http::verb::get && req.target().starts_with("/api/tasks"))
    {
        // get task by id
        size_t base_size = std::string("/api/tasks").length();
        int id = std::stoi(std::string(req.target()).substr(base_size + 1));
        nlohmann::json task = db.getTaskById(id);
        http::response<http::string_body> res{task.empty() ? http::status::not_found : http::status::ok, req.version()};
        res.set(http::field::content_type, "application/json");
        res.body() = task.empty() ? "{\"error\": \"Task not found\"}" : task.dump();
        res.prepare_payload();
        return res;
    }
    else if (req.method() == http::verb::post && req.target() == "/api/tasks")
    {
        // create new task
        nlohmann::json request_body = nlohmann::json::parse(req.body());
        nlohmann::json new_task = db.createTask(request_body);
        http::response<http::string_body> res{http::status::created, req.version()};
        res.set(http::field::content_type, "application/json");
        res.body() = new_task.dump();
        res.prepare_payload();
        return res;
    }
    else if (req.method() == http::verb::put && req.target().starts_with("/api/tasks"))
    {
        // update task by id
        size_t base_size = std::string("/api/tasks").length();
        int id = std::stoi(std::string(req.target()).substr(base_size + 1));
        nlohmann::json request_body = nlohmann::json::parse(req.body());
        nlohmann::json updated_task = db.updateTaskById(id, request_body);
        http::response<http::string_body> res{updated_task.empty() ? http::status::not_found : http::status::ok, req.version()};
        res.set(http::field::content_type, "application/json");
        res.body() = updated_task.empty() ? "{\"error\": \"Task not found\"}" : updated_task.dump();
        res.prepare_payload();
        return res;
    }
    else if (req.method() == http::verb::delete_ && req.target().starts_with("/api/tasks/"))
    {
        // delete task by id (and child tasks if it is)
        size_t base_size = std::string("/api/tasks").length();
        int id = std::stoi(std::string(req.target()).substr(base_size + 1));
        nlohmann::json deleted_task = db.deleteTaskById(id);
        http::response<http::string_body> res{deleted_task.empty() ? http::status::not_found : http::status::ok, req.version()};
        res.set(http::field::content_type, "application/json");
        res.body() = deleted_task.empty() ? "{\"error\": \"Task not found\"}" : deleted_task.dump();
        res.prepare_payload();
        return res;
    }

    // default response for unsupported methods
    return http::response<http::string_body>{http::status::bad_request, req.version()};
}
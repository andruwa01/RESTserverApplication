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
    else if (req.method() == http::verb::post && req.target() == "/api/data")
    {
        // Handle POST request
        nlohmann::json json_request = nlohmann::json::parse(req.body());
        auto json_response = json_request; 

        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        res.body() = json_response.dump();
        res.prepare_payload();
        return res;
    }
    else if (req.method() == http::verb::put && req.target() == "/api/data")
    {
        // Handle PUT request
        auto json_request = nlohmann::json::parse(req.body());
        std::string response_message = "Updated: " + json_request.dump();
        nlohmann::json json_response = {{"message", response_message}};

        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        res.body() = json_response.dump();
        res.prepare_payload();
        return res;
    }
    else if (req.method() == http::verb::delete_ && req.target() == "/api/data")
    {
        // Handle DELETE request
        nlohmann::json json_response = {{"message", "Resource deleted"}};
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Beast");
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        res.body() = json_response.dump();
        res.prepare_payload();
        return res;
    }
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
    else if (req.method() == http::verb::get && req.target().starts_with("/api/employees"))
    {
        std::string target = std::string(req.target());
        size_t last_slash = target.find_last_of('/');
        if (last_slash != std::string::npos && last_slash + 1 < target.size())
        {
            // get employee by id
            int id = std::stoi(target.substr(last_slash + 1));
            nlohmann::json employee = db.getEmployeeById(id);

            http::response<http::string_body> res{employee.empty() ? http::status::not_found : http::status::ok, req.version()};
            res.set(http::field::content_type, "application/json");
            res.body() = employee.empty() ? "{\"error\": \"Employee not found\"}" : employee.dump();
            res.prepare_payload();
            return res;
        }
    }

    // default response for unsupported methods
    return http::response<http::string_body>{http::status::bad_request, req.version()};
}
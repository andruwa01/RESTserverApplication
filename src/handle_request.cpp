#include "requests_handler.hpp"

nlohmann::json create_employee(const nlohmann::json& employee_data, pqxx::connection& conn) {
    pqxx::work txn(conn);
    pqxx::result res = txn.exec_params(
        "INSERT INTO employees (full_name, position) VALUES ($1, $2) RETURNING *",
        employee_data["full_name"].get<std::string>(), employee_data["position"].get<std::string>()
    );
    nlohmann::json new_employee = {
        {"id", res[0]["id"].as<int>()},
        {"full_name", res[0]["full_name"].as<std::string>()},
        {"position", res[0]["position"].as<std::string>()}
    };
    txn.commit();
    return new_employee;
}

nlohmann::json getAllEmployees(Database& db)
{
    pqxx::work txn(db.getConnection());
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

http::response<http::string_body> handle_request(const http::request<http::string_body>& req)
{
    Database db("dbname=tasks_employees user=user password=123 host=localhost port=5432");

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
        nlohmann::json employees = getAllEmployees(db);
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::content_type, "application/json");
        res.body() = employees.dump();
        res.prepare_payload();
        return res;
    }
    else if (req.method() == http::verb::post && req.target() == "/api/employees")
    {
        nlohmann::json request_body = nlohmann::json::parse(req.body());
        nlohmann::json new_employee = create_employee(request_body, db.getConnection());
        http::response<http::string_body> res{http::status::created, req.version()};
        res.set(http::field::content_type, "application/json");
        res.body() = new_employee.dump();
        res.prepare_payload();
        return res;
    }

    // default response for unsupported methods
    return http::response<http::string_body>{http::status::bad_request, req.version()};
}
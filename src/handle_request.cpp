#include "requests_handler.hpp"

http::response<http::string_body> handle_request(const http::request<http::string_body>& req)
{
    if (req.method() == http::verb::get && req.target() == "/api/data")
    {
        Database db("jkds;jfd");

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

    // default response for unsupported methods
    return http::response<http::string_body>{http::status::bad_request, req.version()};
}
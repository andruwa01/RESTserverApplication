// Database.h
#ifndef DATABASE_H_
#define DATABASE_H_

#include <pqxx/pqxx>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>

class Database
{
public:
    Database(const std::string& connection_string);
    ~Database();

    void testConnection();
    pqxx::connection& getConnection();

private:
    pqxx::connection conn_;
};

#endif
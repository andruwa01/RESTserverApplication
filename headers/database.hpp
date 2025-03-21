// Database.h
#ifndef DATABASE_H_
#define DATABASE_H_

#include <pqxx/pqxx>
#include <string>
#include <iostream>

class Database
{
public:
    Database(const std::string& connection_string);
    ~Database();

    void get_one();
    std::string get_all_employees();

private:
    pqxx::connection conn_;
};

#endif
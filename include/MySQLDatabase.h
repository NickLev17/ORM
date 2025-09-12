#ifndef MYSQLDATABASE_H
#define MYSQLDATABASE_H
#include <iostream>
#include <memory>
#include "Database.h"
#include <stdexcept>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

using namespace std;

class MySQLDatabase : public Database
{

    std::unique_ptr<sql::Connection> conn;

public:
    MySQLDatabase(const string &_username, const string &_password);
    ~MySQLDatabase();
    void connect(const string &name, const string &password) override;
    bool isConnected() override;
    void disconnect() override;

    void useDatabase(const string &db_file) override;
    void openDatabase(const string &db_file) override;
    void createDatabase(const string &db_file) override;
    void dropDatabase(const string &db_file) override;

    void createTable(const string &tableName) override;
    void dropTable(const string &tableName) override;
    bool tableExists(const string &tableName) override;
    void renameTable(const string &tableName, const string &newName) override;

    void insert(const std::string &tableName, const User &obj) override;
    std::vector<User> select(const std::string &query) override;
    void update(const std::string &tableName, const User &obj, const std::string &condition) override;
    void removeObject(const std::string &tableName, const std::string &condition) override;
    std::vector<User> getAllObjects(const std::string &tableName) override;
    std::vector<User> getObjects(const std::string &tableName, const string &condition) override;
    void removeAllObjects(const std::string &tableName) override;
    std::vector<User> getObjectsOrderBy(const std::string &tableName, const string &field) override;
    std::vector<User> getObjectsOrderBy(const std::string &tableName, const string &field1, const string &field2) override;

    void beginTransaction() override;
    void commit() override;
    void rollback() override;

private:
    void close();
    bool execute(const string &sql) override;
};

#endif
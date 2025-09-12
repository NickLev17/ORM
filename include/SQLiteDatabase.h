#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H
#include <iostream>
#include "Database.h"
#include <cstdio>
#include <sqlite3.h>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

using namespace std;

class SQLiteDatabase : public Database
{
    sqlite3 *db = nullptr;
    bool connection = false;

public:
    SQLiteDatabase();
    ~SQLiteDatabase();

    bool isConnected() override;
    void disconnect() override;
    void connect(const string &, const string &) override;

    void openDatabase(const string &db_file) override;
    void createDatabase(const string &db_file) override;
    void useDatabase(const string &db_file) override;
    void dropDatabase(const string &db_file) override;

    bool tableExists(const string &tableName) override;
    void dropTable(const string &tableName) override;
    void createTable(const string &tableName) override;
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

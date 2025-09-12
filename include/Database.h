#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct User
{
    int id;
    std::string name;
    int age;
};

class Database
{

public:
    Database() {};
    virtual ~Database() = default;

    Database(const Database &) = delete;
    Database &operator=(const Database &) = delete;

    virtual bool execute(const string &sql) = 0;

    virtual void connect(const string &name, const string &password) = 0;
    virtual bool isConnected() = 0;
    virtual void disconnect() = 0;

    // Work database
    virtual void dropDatabase(const string &db_file) = 0;
    virtual void openDatabase(const string &db_file) = 0;
    virtual void createDatabase(const string &db_file) = 0;
    virtual void useDatabase(const string &db_file) = 0;

    // Work table
    virtual bool tableExists(const string &tableName) = 0;
    virtual void createTable(const string &tableName) = 0;
    virtual void dropTable(const string &tableName) = 0;
    virtual void renameTable(const string &tableName, const string &newName) = 0;

    // Mapping data (CRUD, ORDER)
    virtual void insert(const std::string &tableName, const User &obj) = 0;
    virtual std::vector<User> select(const std::string &query) = 0;
    virtual std::vector<User> getAllObjects(const std::string &tableName) = 0;
    virtual std::vector<User> getObjects(const std::string &tableName, const string &condition) = 0;
    virtual void update(const std::string &tableName, const User &obj, const std::string &condition) = 0;
    virtual void removeObject(const std::string &tableName, const std::string &condition) = 0;
    virtual void removeAllObjects(const std::string &tableName) = 0;
    virtual std::vector<User> getObjectsOrderBy(const std::string &tableName, const string &field) = 0;
    virtual std::vector<User> getObjectsOrderBy(const std::string &tableName, const string &field1, const string &field2) = 0;

    // Client desides
    virtual void beginTransaction() = 0;
    virtual void commit() = 0;
    virtual void rollback() = 0;
};

#endif
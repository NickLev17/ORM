// #include "SQLiteDatabase.h"
#include "../include/SQLiteDatabase.h"
SQLiteDatabase::SQLiteDatabase()
{
}

SQLiteDatabase::~SQLiteDatabase()
{
    if (isConnected())
    {
        disconnect();
    }

    db = nullptr;
}
void SQLiteDatabase::connect(const string &, const string &)
{
}

void SQLiteDatabase::openDatabase(const string &db_file)
{
    if (sqlite3_open(db_file.c_str(), &db))
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << "\n";
        db = nullptr;
        connection = false;
    }
    connection = true;
}
void SQLiteDatabase::createDatabase(const string &db_file)
{
    if (sqlite3_open(db_file.c_str(), &db))
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << "\n";
        db = nullptr;
        connection = false;
    }
    connection = true;
}
bool SQLiteDatabase::isConnected()
{
    return connection;
}
void SQLiteDatabase::disconnect()
{
    close();
}
void SQLiteDatabase::useDatabase(const string &db_file)
{

    if (db != nullptr)
    {
        sqlite3_close(db);
        db = nullptr;
    }

    int rc = sqlite3_open(db_file.c_str(), &db);

    if (rc == SQLITE_OK)
    {
        std::cout << "Database opened successfully: " << db_file << "\n";
    }
    else
    {
        std::cout << "Can't open database: " << sqlite3_errmsg(db) << "\n";
    }
}
bool SQLiteDatabase::tableExists(const string &tableName)
{
    string sql = "SELECT count(name) FROM sqlite_master WHERE type='table' AND name='" + tableName + "'";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return false;
    int rc = sqlite3_step(stmt);
    bool exists = false;

    if (rc == SQLITE_ROW)
    {
        int count = sqlite3_column_int(stmt, 0);
        exists = (count == 1);
    }
    sqlite3_finalize(stmt);
    return exists;
}
void SQLiteDatabase::dropDatabase(const string &db_file)
{
    disconnect();
    int res = std::remove(db_file.c_str());
    if (res == 0)
    {

        cout << " Database " << db_file << " wos deleted successfully\n";
    }
    else
    {
        cout << " Database " << db_file << " wos not delete\n";
    }
}
void SQLiteDatabase::dropTable(const string &tableName)
{
    string sql = "DROP TABLE `" + tableName + "`";

    bool result = execute(sql);
    if (result)
    {
        cout << " Table " << tableName << " wos deleted succeccfully\n";
    }
    else
    {
        {
            cout << " Table " << tableName << " wos not deleted \n";
        }
    }
}
bool SQLiteDatabase::execute(const string &sql)
{

    char *errmsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errmsg << "\n";
        sqlite3_free(errmsg);
        return false;
    }
    return true;
}
void SQLiteDatabase::createTable(const string &tableName)
{

    std::string sql = "CREATE TABLE IF NOT EXISTS `" + tableName + "` ("
                                                                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                                   "name TEXT NOT NULL,"
                                                                   "age INTEGER NOT NULL);";

    bool result = execute(sql);
    if (result)
    {
        cout << " Table " << tableName << " wos created succeccfully\n";
    }
    else
    {

        cout << " Table " << tableName << " wos not created\n";
    }
}
void SQLiteDatabase::renameTable(const string &tableName, const string &newName)
{
    std::string sql = "ALTER TABLE `" + tableName + "` RENAME TO `" + newName + "`;";
    // std::string sql = "ALTER TABLE `" + tableName + "` RENAME TO " + newName + ";";
    bool result = execute(sql);
    if (result)
    {
        cout << " Table " << tableName << " wos renamed succeccfully\n";
    }
    else
    {

        cout << " Table " << tableName << " wos not renamed\n";
    }
}

void SQLiteDatabase::insert(const std::string &tableName, const User &obj)
{
    auto id = obj.id;
    auto age = obj.age;
    auto name = obj.name;
    std::string sql = "INSERT INTO `" + tableName + "` VALUES(" + std::to_string(id) + ", '" + name + "', " + std::to_string(age) + ");";
    bool result = execute(sql);
    if (result)
    {
        cout << " Data wos inserted successfully\n";
    }
    else
    {
        cout << " Data wos not inserted secessfuly\n";
    }
}

std::vector<User> SQLiteDatabase::select(const std::string &query)
{
    std::vector<User> users;
    auto tmp = query;
    const char *sql = tmp.c_str();
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement\n";
        return users;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        User u;
        u.id = sqlite3_column_int(stmt, 0);
        u.name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        u.age = sqlite3_column_int(stmt, 2);
        users.push_back(u);
    }

    sqlite3_finalize(stmt);
    return users;
}

std::vector<User> SQLiteDatabase::getAllObjects(const std::string &tableName)
{
    std::vector<User> users;
    std::string tmp = "SELECT * FROM `" + tableName + "` ;";
    ;
    const char *sql = tmp.c_str();
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement\n";
        return users;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        User u;
        u.id = sqlite3_column_int(stmt, 0);
        u.name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        u.age = sqlite3_column_int(stmt, 2);
        users.push_back(u);
    }

    sqlite3_finalize(stmt);
    return users;
}

std::vector<User> SQLiteDatabase::getObjects(const std::string &tableName, const string &condition)
{
    std::vector<User> users;
    std::string tmp = "SELECT * FROM `" + tableName + "` WHERE " + condition + ";";
    ;
    const char *sql = tmp.c_str();
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement\n";
        return users;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        User u;
        u.id = sqlite3_column_int(stmt, 0);
        u.name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        u.age = sqlite3_column_int(stmt, 2);
        users.push_back(u);
    }

    sqlite3_finalize(stmt);
    return users;
}

void SQLiteDatabase::update(const std::string &tableName, const User &obj, const std::string &condition)
{
    std::string sql = "UPDATE `" + tableName + "` SET "
                                               "name = '" +
                      obj.name + "', "
                                 "age = " +
                      std::to_string(obj.age) + ", id= " + std::to_string(obj.id) + " "
                                                                                    "WHERE " +
                      condition + ";";

    bool result = execute(sql);
    if (result)
    {
        cout << "  Data wos updated successfully\n";
    }
    else
    {
        cout << " Data wos not updated successfully\n";
    }
}

void SQLiteDatabase::removeObject(const std::string &tableName, const std::string &condition)
{
    std::string sql = "DELETE FROM `" + tableName + "` WHERE " + condition + ";";

    bool result = execute(sql);
    if (result)
    {
        cout << " Data wos deleted successfully\n";
    }
    else
    {
        cout << " Data wos not successfully\n";
    }
}

void SQLiteDatabase::removeAllObjects(const std::string &tableName)
{
    std::string sql = "DELETE FROM `" + tableName + "` ;";

    bool result = execute(sql);
    if (result)
    {
        cout << " Data wos deleted successfully\n";
    }
    else
    {
        cout << " Data wos not successfully\n";
    }
}

std::vector<User> SQLiteDatabase::getObjectsOrderBy(const std::string &tableName, const string &field)
{

    std::string tmp = "SELECT * FROM `" + tableName + "` ORDER BY " + field + ";";

    std::vector<User> users;
    const char *sql = tmp.c_str();
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement\n";
        return users;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        User u;
        u.id = sqlite3_column_int(stmt, 0);
        u.name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        u.age = sqlite3_column_int(stmt, 2);
        users.push_back(u);
    }

    sqlite3_finalize(stmt);
    return users;
}

std::vector<User> SQLiteDatabase::getObjectsOrderBy(const std::string &tableName, const string &field1, const string &field2)
{

    std::string tmp = "SELECT * FROM `" + tableName + "` ORDER BY " + field1 + " , " + field2 + ";";

    std::vector<User> users;
    const char *sql = tmp.c_str();
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement\n";
        return users;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        User u;
        u.id = sqlite3_column_int(stmt, 0);
        u.name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        u.age = sqlite3_column_int(stmt, 2);
        users.push_back(u);
    }

    sqlite3_finalize(stmt);
    return users;
}

void SQLiteDatabase::close()
{
    if (db)
    {
        sqlite3_close(db);
        connection = false;
    }
}

void SQLiteDatabase::beginTransaction()
{
    string sql = "BEGIN TRANSACTION;";
    bool result = execute(sql);
    if (result)
    {
        cout << " Start of transaction\n";
    }
    else
    {
        cout << " Failed start of transction\n";
    }
}
void SQLiteDatabase::commit()
{
    string sql = "COMMIT;";
    bool result = execute(sql);
    if (result)
    {
        cout << " Commit\n";
    }
    else
    {
        cout << " Failed commit\n";
    }
}
void SQLiteDatabase::rollback()
{
    string sql = "ROLLBACK;";
    bool result = execute(sql);
    if (result)
    {
        cout << " Rollback\n";
    }
    else
    {
        cout << " Failed rollback\n";
    }
}
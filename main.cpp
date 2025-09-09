#include <iostream>
#include <cstdio>
#include <sqlite3.h>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
using namespace std;

class Database
{
    // Database *db;
public:
    Database() {};
    virtual ~Database() = default;

    //  разделить методы на публичные и приватные
    virtual void close() = 0;

    virtual bool execute(const string &sql) = 0;

    // virtual void connect(const string& connectionString)=0;
    // virtual void disconnect()=0;
    // virtual bool isConnected()=0;

    // работа с БД
    virtual void dropDatabase(const string &db_file) = 0;
    virtual void openDatabase(const string &db_file) = 0; //
    virtual void createDatabase(const string &db_file) = 0;
    virtual void useDatabase(const string &db_file) = 0;

    // работа с таблицей
    virtual bool tableExists(const string &tableName) = 0;
    virtual void createTable(const string &tableName) = 0;
    virtual void dropTable(const string &tableName) = 0;

    // ДЛя маппинга данных
    // template<typename T>
    // virtual bool insert(const T& obj) = 0;

    // template<typename T>
    // virtual std::vector<T> select(const std::string& query) = 0;

    // template<typename T>
    // virtual bool update(const T& obj, const std::string& condition) = 0;

    // template<typename T>
    // virtual bool remove(const std::string& condition) = 0;

    // работа с данными
    // virtual void insert(const std::string& tableName, const Object& obj) = 0;
    // virtual std::vector<Object> select(const std::string& query) = 0;
    // virtual void update(const std::string& tableName, const Object& obj, const std::string& condition) = 0;
    // virtual void remove(const std::string& tableName, const std::string& condition) = 0;

    //****** */
    // virtual void beginTransaction()=0;
    // virtual void commit()=0;
    // virtual void rollback()=0;
};

class MySQLDatabase : public Database
{

    std::unique_ptr<sql::Connection> conn;

public:
    MySQLDatabase()
    {
        string user = "root";
        string password = "simple1";
        // const std::string &db = ""
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        conn.reset(driver->connect("tcp://127.0.0.1:3306", user, password));
    }

    void useDatabase(const string &db_file) override
    {
        std::unique_ptr<sql::Statement> stmt(conn.get()->createStatement());
        stmt.get()->execute("USE `" + db_file + "`");
        // добавить обработку исключения при выборе БД которой не существует
    }
    void dropTable(const string &tableName) override
    {
        std::unique_ptr<sql::Statement> stmt(conn.get()->createStatement());
        stmt.get()->execute("DROP TABLE `" + tableName + "`");
    }

    bool tableExists(const string &tableName)
    {
        std::unique_ptr<sql::Statement> stmt(conn.get()->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt.get()->executeQuery("SHOW TABLES LIKE '" + tableName + "'"));

        if (res.get()->next())
        {

            return true;
        }

        return false;
    }
    void openDatabase(const string &db_file) override
    {

        if (!db_file.empty())
        {

            std::unique_ptr<sql::Statement> stmt(conn.get()->createStatement());
            std::unique_ptr<sql::ResultSet> res(stmt.get()->executeQuery("SHOW DATABASES LIKE '" + db_file + "'"));

            if (!res.get()->next())
            {

                createDatabase(db_file);
            }
            else
            {

                conn.get()->setSchema(db_file);
            }
        }
    }
    void createDatabase(const string &db_file) override
    {
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        stmt->execute("CREATE DATABASE IF NOT EXISTS " + db_file);
        conn->setSchema(db_file);
    }
    void dropDatabase(const string &db_file) override
    {
        std::unique_ptr<sql::Statement> stmt(conn.get()->createStatement());

        if (!db_file.empty())
        {

            stmt.get()->execute("DROP DATABASE IF EXISTS `" + db_file + "`");
        }
        else
        {

            cout << " DB-file name EMPTY  ****\n";
        }
    }

    bool execute(const string &sql) override
    {
        return true;
    }

    void createTable(const string &tableName) override
    {
        std::unique_ptr<sql::Statement> stmt(conn.get()->createStatement());
        std::string sql = "CREATE TABLE IF NOT EXISTS `" + tableName + "` ("
                                                                       "id INT AUTO_INCREMENT PRIMARY KEY,"
                                                                       "name VARCHAR(255) NOT NULL,"
                                                                       "age INT NOT NULL);";
        stmt.get()->execute(sql);
    }

    void close() override
    {
        conn.get()->close();
    }
};

class SQLiteDatabase : public Database
{
    sqlite3 *db = nullptr;

public:
    SQLiteDatabase()
    {
    }

    void openDatabase(const string &db_file) override

    {
        if (sqlite3_open(db_file.c_str(), &db))
        {
            cout << " 1\n";
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << "\n";
            db = nullptr;
        }
    }
    void createDatabase(const string &db_file) override
    {
        if (sqlite3_open(db_file.c_str(), &db))
        {
            cout << " 1\n";
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << "\n";
            db = nullptr;
        }
    }
    void useDatabase(const string &db_file) override
    {
        // переделать , чтобы SQLite  не создавал файл, а boost пробегался про директории имеющихся файлов

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
    bool tableExists(const string &tableName)
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
    void dropDatabase(const string &db_file) override
    {

        std::remove(db_file.c_str());
    }
    void dropTable(const string &tableName) override
    {
        string sql = "DROP TABLE `" + tableName + "`";

        int result = execute(sql); // доделать проверку
    }
    bool execute(const string &sql) override
    {

        char *errmsg = nullptr;
        cout << " 2\n";
        int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errmsg);
        if (rc != SQLITE_OK)
        {
            cout << " 3\n";
            std::cerr << "SQL error: " << errmsg << "\n";
            sqlite3_free(errmsg);
            return false;
        }
        return true;
    }
    void createTable(const string &tableName) override

    {

        std::string sql = "CREATE TABLE IF NOT EXISTS `" + tableName + "` ("
                                                                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                                       "name TEXT NOT NULL,"
                                                                       "age INTEGER NOT NULL);";

        cout << " 4\n";

        int result = execute(sql); // добавить проверку
    }

    void close() override
    {
        if (db)
        {
            sqlite3_close(db);
        }
    }
};

int main()
{

    Database *db_sql = new SQLiteDatabase();
    db_sql->openDatabase("employee");
    db_sql->createTable("people");
    cout << "Exist table ? " << db_sql->tableExists("qwerty") << "\n";
    cout << "Exist table ? " << db_sql->tableExists("people") << "\n";

    Database *db_mysql = new MySQLDatabase();
    db_mysql->openDatabase("employee");
    db_mysql->createTable("people");
    cout << "Exist table ? " << db_mysql->tableExists("qwerty") << "\n";
    cout << "Exist table ? " << db_mysql->tableExists("people") << "\n";

    return 0;
}
// g++ main.cpp -o main -lmysqlcppconn -lsqlite3
#include "../include/MySQLDatabase.h"

MySQLDatabase::MySQLDatabase(const string &_username, const string &_password)
{
    connect(_username, _password);
}
MySQLDatabase::~MySQLDatabase()
{
    if (isConnected())
    {
        disconnect();
    }
}

void MySQLDatabase::connect(const string &name, const string &password)
{
    if (isConnected())
    {
        disconnect();
    }
    string user = name;
    string _password = password;
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    conn.reset(driver->connect("tcp://127.0.0.1:3306", user, _password));
}
void MySQLDatabase::disconnect()
{
    close();
}

bool MySQLDatabase::isConnected()
{
    return conn && conn->isValid();
}
void MySQLDatabase::useDatabase(const string &db_file)
{
    std::string sql = "USE `" + db_file + "` ;";
    bool res = execute(sql);
    if (res)
    {
        cout << "Use " << db_file << " change secuess\n";
    }
    else
    {
        cout << "Database wos not changed\n";
    }
}
void MySQLDatabase::dropTable(const string &tableName)
{
    std::string sql = "DROP TABLE `" + tableName + "` ;";
    bool res = execute(sql);
    if (res)
    {
        cout << "Table " << tableName << " wos droped successfully\n";
    }
    else
    {
        cout << "Table wos not droped\n";
    }
}

bool MySQLDatabase::tableExists(const string &tableName)
{
    std::unique_ptr<sql::Statement> stmt(conn.get()->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt.get()->executeQuery("SHOW TABLES LIKE '" + tableName + "'"));

    if (res.get()->next())
    {

        return true;
    }

    return false;
}
void MySQLDatabase::openDatabase(const string &db_file)
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
void MySQLDatabase::createDatabase(const string &db_file)
{
    std::unique_ptr<sql::Statement> stmt(conn->createStatement());
    stmt->execute("CREATE DATABASE IF NOT EXISTS " + db_file);
    conn->setSchema(db_file);
}
void MySQLDatabase::dropDatabase(const string &db_file)
{
    std::unique_ptr<sql::Statement> stmt(conn.get()->createStatement());

    if (!db_file.empty())
    {

        stmt.get()->execute("DROP DATABASE IF EXISTS `" + db_file + "`");
    }
    else
    {
        cout << "Not drop db_file \n";
    }
}

bool MySQLDatabase::execute(const string &sql)
{
    std::unique_ptr<sql::Statement> stmt(conn.get()->createStatement());
    bool result = stmt.get()->execute(sql);
    if (!result)
    {
        return true;
    }
    return false;
}

void MySQLDatabase::createTable(const string &tableName)
{
    std::unique_ptr<sql::Statement> stmt(conn.get()->createStatement());
    std::string sql = "CREATE TABLE IF NOT EXISTS `" + tableName + "` ("
                                                                   "id INT AUTO_INCREMENT PRIMARY KEY,"
                                                                   "name VARCHAR(255) NOT NULL,"
                                                                   "age INT NOT NULL);";
    stmt.get()->execute(sql);
}

void MySQLDatabase::renameTable(const string &tableName, const string &newName)
{
    std::string sql = "ALTER TABLE `" + tableName + "` RENAME TO " + newName + ";";

    bool res = execute(sql);
    if (res)
    {
        cout << "Table wos renamed successfully\n";
    }
    else
    {
        cout << "Tale wos not renamed\n";
    }
}

void MySQLDatabase::insert(const std::string &tableName, const User &obj)
{
    auto id = obj.id;
    auto age = obj.age;
    auto name = obj.name;
    std::string sql = "INSERT INTO `" + tableName + "` VALUES(" + std::to_string(id) + ", '" + name + "', " + std::to_string(age) + ");";

    bool res = execute(sql);
    if (res)
    {
        cout << "Data wos inserted successfully\n";
    }
    else
    {
        cout << "Data wos not inserted\n";
    }
}
std::vector<User> MySQLDatabase::select(const std::string &query)
{
    vector<User> tmp;

    std::unique_ptr<sql::Statement> stmt(conn.get()->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt.get()->executeQuery(query));

    while (res.get()->next())
    {
        User us;
        us.id = res->getInt("id");
        us.name = res->getString("name");
        us.age = res->getInt("age");
        tmp.push_back(us);
    }
    return tmp;
}

std::vector<User> MySQLDatabase::getAllObjects(const std::string &tableName)
{
    std::string sql = "SELECT * FROM `" + tableName + "` ;";

    vector<User> tmp;

    std::unique_ptr<sql::Statement> stmt(conn.get()->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt.get()->executeQuery(sql));

    while (res.get()->next())
    {
        User us;
        us.id = res->getInt("id");
        us.name = res->getString("name");
        us.age = res->getInt("age");
        tmp.push_back(us);
    }
    return tmp;
}

std::vector<User> MySQLDatabase::getObjectsOrderBy(const std::string &tableName, const string &field)
{
    std::string sql = "SELECT * FROM `" + tableName + "` ORDER BY " + field + " ;";

    vector<User> tmp;

    std::unique_ptr<sql::Statement> stmt(conn.get()->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt.get()->executeQuery(sql));

    while (res.get()->next())
    {
        User us;
        us.id = res->getInt("id");
        us.name = res->getString("name");
        us.age = res->getInt("age");
        tmp.push_back(us);
    }
    return tmp;
}

std::vector<User> MySQLDatabase::getObjectsOrderBy(const std::string &tableName, const string &field1, const string &field2)
{
    std::string sql = "SELECT * FROM `" + tableName + "` ORDER BY " + field1 + " , " + field2 + " ;";

    vector<User> tmp;

    std::unique_ptr<sql::Statement> stmt(conn.get()->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt.get()->executeQuery(sql));

    while (res.get()->next())
    {
        User us;
        us.id = res->getInt("id");
        us.name = res->getString("name");
        us.age = res->getInt("age");
        tmp.push_back(us);
    }

    return tmp;
}

std::vector<User> MySQLDatabase::getObjects(const std::string &tableName, const string &condition)
{
    std::string sql = "SELECT * FROM `" + tableName + "` WHERE " + condition + ";";
    vector<User> tmp;

    std::unique_ptr<sql::Statement> stmt(conn.get()->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt.get()->executeQuery(sql));

    while (res.get()->next())
    {
        User us;
        us.id = res->getInt("id");
        us.name = res->getString("name");
        us.age = res->getInt("age");
        tmp.push_back(us);
    }

    return tmp;
}

void MySQLDatabase::update(const std::string &tableName, const User &obj, const std::string &condition)
{
    try
    {
        std::string sql = "UPDATE `" + tableName + "` SET "
                                                   "name = '" +
                          obj.name + "', "
                                     "age = " +
                          std::to_string(obj.age) + ", id= " + std::to_string(obj.id) + " "
                                                                                        "WHERE " +
                          condition + ";";

        // Создаем statement и выполняем запрос
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        stmt.get()->executeUpdate(sql);

        std::cout << "Data wos updated successfully " << std::endl;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Data wos not updated " << e.what() << std::endl;
    }
}

void MySQLDatabase::removeObject(const std::string &tableName, const std::string &condition)
{

    string sql = "DELETE FROM `" + tableName + "`  WHERE " + condition + " ;";

    bool res = execute(sql);
    if (res)
    {
        cout << "Data wos removed successfully\n";
    }
    else
    {
        cout << "Data wos not removed\n";
    }
}
void MySQLDatabase::removeAllObjects(const std::string &tableName)
{
    string sql = "DELETE FROM `" + tableName + "` ;";

    bool res = execute(sql);
    if (res)
    {
        cout << "Data wos removed successfully\n";
    }
    else
    {
        cout << "Data wos not removed\n";
    }
}
void MySQLDatabase::close()
{
    conn.get()->close();
}

void MySQLDatabase::beginTransaction()
{
    string sql = "START TRANSACTION;";
    bool res = execute(sql);

    if (res)
    {
        cout << "START TRANSACTION " << "\n";
    }
    else
    {
        cout << "Failed START TRANSACTION " << "\n";
    }
}
void MySQLDatabase::commit()
{
    string sql = "COMMIT;";

    bool res = execute(sql);
    if (res)
    {
        cout << "COMMIT " << "\n";
    }
    else
    {
        cout << " Failed COMMIT " << "\n";
    }
}
void MySQLDatabase::rollback()
{

    string sql = "ROLLBACK;";
    bool res = execute(sql);
    if (res)
    {
        cout << "ROLLBACK " << "\n";
    }
    else
    {
        cout << "Failed ROLLBACK " << "\n";
    }
}

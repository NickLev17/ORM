#include <gtest/gtest.h>
#include "../include/SQLiteDatabase.h"

class SQLiteDatabaseTest : public ::testing::Test
{
protected:
    SQLiteDatabase db;
    const std::string test_db = ":memory:";

    void SetUp() override
    {
        db.openDatabase(test_db);

        ASSERT_TRUE(db.isConnected());
    }

    void TearDown() override
    {

        db.close();
    }
};

TEST_F(SQLiteDatabaseTest, isCOnnected)
{
    ASSERT_TRUE(db.isConnected());

    db.close();
    ASSERT_FALSE(db.isConnected());
}

TEST_F(SQLiteDatabaseTest, CreateDatabase)
{

    ASSERT_TRUE(db.isConnected());
}
TEST_F(SQLiteDatabaseTest, OpenDatabase)
{

    ASSERT_TRUE(db.isConnected());
}

TEST_F(SQLiteDatabaseTest, execute)
{
    ASSERT_FALSE(db.tableExists("users"));
    std::string sql = "CREATE TABLE IF NOT EXISTS `users` ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "name TEXT NOT NULL,"
                      "age INTEGER NOT NULL);";
    bool result = db.execute(sql);

    ASSERT_TRUE(result);
    ASSERT_TRUE(db.tableExists("users"));
}

TEST_F(SQLiteDatabaseTest, UseDatabase)
{
    db.useDatabase(test_db);
    ASSERT_TRUE(db.isConnected());
}

TEST_F(SQLiteDatabaseTest, CreateTable)
{
    ASSERT_FALSE(db.tableExists("users"));
    db.createTable("users");
    ASSERT_TRUE(db.tableExists("users"));
}

TEST_F(SQLiteDatabaseTest, DropTable)
{
    db.createTable("users");
    ASSERT_TRUE(db.tableExists("users"));
    db.dropTable("users");
    ASSERT_FALSE(db.tableExists("users"));
}

TEST_F(SQLiteDatabaseTest, Insert)
{
    db.createTable("users");

    User u1{1, "Anna", 30};
    User u2{2, "Pavel", 35};
    db.insert("users", u1);
    db.insert("users", u2);

    auto users = db.getAllObjects("users");
    ASSERT_EQ(users.size(), 2);
    EXPECT_EQ(users[0].name, "Anna");
    EXPECT_EQ(users[0].age, 30);
    EXPECT_EQ(users[1].name, "Pavel");
    EXPECT_EQ(users[1].age, 35);
}

TEST_F(SQLiteDatabaseTest, Update)
{
    db.createTable("users");
    User u1{1, "Sam", 30};
    User u2{2, "Bob", 35};
    db.insert("users", u1);
    db.insert("users", u2);
    auto users = db.getAllObjects("users");
    ASSERT_EQ(users.size(), 2);

    User u3{1, "Sam Updated", 31};
    db.update("users", u3, "id=1");
    users = db.getObjects("users", "id=1");
    ASSERT_EQ(users.at(0).name, "Sam Updated");
    ASSERT_EQ(users.at(0).age, 31);
}

TEST_F(SQLiteDatabaseTest, RemoveObject)
{
    db.createTable("users");
    User u1{1, "Sam", 30};
    User u2{2, "Bob", 35};
    db.insert("users", u1);
    db.insert("users", u2);
    auto users = db.getAllObjects("users");
    ASSERT_EQ(users.size(), 2);
    db.removeObject("users", "id=1");
    db.removeObject("users", "name='Bob'");
    auto users2AfterRemove = db.getAllObjects("users");
    ASSERT_TRUE(users2AfterRemove.empty());
}

TEST_F(SQLiteDatabaseTest, Select)
{
    db.createTable("users");

    User u1{1, "Anna", 31};
    User u2{2, "Bob", 30};
    db.insert("users", u1);
    db.insert("users", u2);
    auto resultSelect = db.select("SELECT * FROM users");
    ASSERT_EQ(resultSelect.size(), 2);
    ASSERT_EQ(u1.name, resultSelect.at(0).name);
    ASSERT_EQ(u1.age, resultSelect.at(0).age);
    ASSERT_EQ(u2.name, resultSelect.at(1).name);
    ASSERT_EQ(u2.age, resultSelect.at(1).age);
}

TEST_F(SQLiteDatabaseTest, TransactionsCommitRollback)
{
    db.createTable("users");
    db.beginTransaction();

    User u1{1, "Bob", 25};
    db.insert("users", u1);

    db.rollback();

    auto users = db.getAllObjects("users");
    ASSERT_TRUE(users.empty());

    db.beginTransaction();
    db.insert("users", u1);
    db.commit();

    users = db.getAllObjects("users");
    ASSERT_EQ(users.size(), 1);
}

TEST_F(SQLiteDatabaseTest, close)
{
    db.close();
    EXPECT_FALSE(db.isConnected());
}

TEST_F(SQLiteDatabaseTest, getAllObjects)
{
    db.createTable("Users");
    User u1{1, "Alice", 30};
    User u2{2, "Bob", 40};
    User u3{3, "Anrey", 20};

    db.insert("Users", u1);
    db.insert("Users", u2);
    db.insert("Users", u3);

    auto users = db.getAllObjects("Users");
    ASSERT_EQ(users.size(), 3);
    ASSERT_EQ(u1.id, users.at(0).id);
    ASSERT_EQ(u1.name, users.at(0).name);
    ASSERT_EQ(u1.age, users.at(0).age);

    ASSERT_EQ(u2.id, users.at(1).id);
    ASSERT_EQ(u2.name, users.at(1).name);
    ASSERT_EQ(u2.age, users.at(1).age);

    ASSERT_EQ(u3.id, users.at(2).id);
    ASSERT_EQ(u3.name, users.at(2).name);
    ASSERT_EQ(u3.age, users.at(2).age);
}

TEST_F(SQLiteDatabaseTest, getObjects)
{
    db.createTable("Users");
    User u1{1, "Alice", 30};
    User u2{2, "Bob", 40};
    User u3{3, "Anrey", 20};

    db.insert("Users", u1);
    db.insert("Users", u2);
    db.insert("Users", u3);

    auto users = db.getObjects("Users", "age>=30");
    ASSERT_EQ(users.size(), 2);
    ASSERT_EQ(u1.id, users.at(0).id);
    ASSERT_EQ(u1.name, users.at(0).name);
    ASSERT_EQ(u1.age, users.at(0).age);

    ASSERT_EQ(u2.id, users.at(1).id);
    ASSERT_EQ(u2.name, users.at(1).name);
    ASSERT_EQ(u2.age, users.at(1).age);
}

TEST_F(SQLiteDatabaseTest, removeAllObjects)
{
    db.createTable("Users");
    User u1{1, "Alice", 30};
    User u2{2, "Bob", 40};
    User u3{3, "Anrey", 20};

    db.insert("Users", u1);
    db.insert("Users", u2);
    db.insert("Users", u3);

    auto users = db.getAllObjects("Users");
    ASSERT_TRUE(!users.empty());

    db.removeAllObjects("Users");
    auto users2 = db.getAllObjects("Users");

    ASSERT_TRUE(users2.empty());
}

TEST_F(SQLiteDatabaseTest, renameTable)
{
    db.createTable("Users");
    ASSERT_TRUE(db.tableExists("Users"));

    db.renameTable("Users", "NewUsers");
    ASSERT_TRUE(db.tableExists("NewUsers"));
    ASSERT_FALSE(db.tableExists("Users"));
}

TEST_F(SQLiteDatabaseTest, disconnect)
{
    ASSERT_TRUE(db.isConnected());
    db.close();
    ASSERT_FALSE(db.isConnected());
}

TEST_F(SQLiteDatabaseTest, tableExists)
{

    ASSERT_FALSE(db.tableExists("Users"));

    db.createTable("Users");
    ASSERT_TRUE(db.tableExists("Users"));
}

TEST_F(SQLiteDatabaseTest, getObjectsOrderByField1)
{

    db.createTable("Users");
    User u1{1, "Bob", 42};
    User u2{2, "Sam", 25};
    User u3{3, "Arina", 35};
    User u4{4, "Anna", 30};

    db.insert("Users", u1);
    db.insert("Users", u2);
    db.insert("Users", u3);
    db.insert("Users", u4);
    auto user = db.getObjectsOrderBy("Users", "name");

    ASSERT_EQ(u4.name, user.at(0).name);
    ASSERT_EQ(u3.name, user.at(1).name);
    ASSERT_EQ(u1.name, user.at(2).name);
    ASSERT_EQ(u2.name, user.at(3).name);
}

TEST_F(SQLiteDatabaseTest, getObjectsOrderByField1AndField2)
{

    db.createTable("Users");
    User u1{1, "Bob", 42};
    User u3{2, "Anna", 35};
    User u2{3, "Bob", 25};
    User u4{4, "Anna", 30};

    db.insert("Users", u1);
    db.insert("Users", u2);
    db.insert("Users", u3);
    db.insert("Users", u4);
    auto user = db.getObjectsOrderBy("Users", "name", "age");

    ASSERT_EQ(u4.name, user.at(0).name);
    ASSERT_EQ(u3.name, user.at(1).name);
    ASSERT_EQ(u2.name, user.at(2).name);
    ASSERT_EQ(u1.name, user.at(3).name);
}

TEST_F(SQLiteDatabaseTest, getTables)
{
    EXPECT_FALSE(db.tableExists("Users"));
    EXPECT_FALSE(db.tableExists("Example"));

    db.createTable("Users");
    db.createTable("Example");

    auto tables = db.getTables();
    ASSERT_EQ(tables.size(), 2);
    ASSERT_EQ("Users", tables.at(0));
    ASSERT_EQ("Example", tables.at(1));
}

TEST_F(SQLiteDatabaseTest, copyAllObjectsInOtherTable)
{
    EXPECT_FALSE(db.tableExists("Users"));
    EXPECT_FALSE(db.tableExists("Example"));

    db.createTable("Users");
    db.createTable("Example");

    User u1{1, "Sam", 25};
    User u2{2, "Bob", 26};
    User u3{3, "Anna", 21};

    db.insert("Users", u1);
    db.insert("Users", u2);
    db.insert("Users", u3);

    db.copyAllObjectsInOtherTable("Users", "Example");
    EXPECT_EQ(db.getAllObjects("Users").size(), db.getAllObjects("Example").size());

    auto userFromExample = db.getAllObjects("Example");
    ASSERT_EQ(u1.id, userFromExample.at(0).id);
    ASSERT_EQ(u1.age, userFromExample.at(0).age);
    ASSERT_EQ(u1.name, userFromExample.at(0).name);

    ASSERT_EQ(u2.id, userFromExample.at(1).id);
    ASSERT_EQ(u2.age, userFromExample.at(1).age);
    ASSERT_EQ(u2.name, userFromExample.at(1).name);

    ASSERT_EQ(u3.id, userFromExample.at(2).id);
    ASSERT_EQ(u3.age, userFromExample.at(2).age);
    ASSERT_EQ(u3.name, userFromExample.at(2).name);
}

TEST_F(SQLiteDatabaseTest, copyObjectsInOtherTable)
{
    EXPECT_FALSE(db.tableExists("Users"));
    EXPECT_FALSE(db.tableExists("Example"));

    db.createTable("Users");
    db.createTable("Example");

    User u1{1, "Sam", 25};
    User u2{2, "Bob", 26};
    User u3{3, "Anna", 21};

    db.insert("Users", u1);
    db.insert("Users", u2);
    db.insert("Users", u3);

    db.copyObjectsInOtherTable("Users", "Example", "name='Bob'");
    db.copyObjectsInOtherTable("Users", "Example", "age=21");

    EXPECT_TRUE(db.getAllObjects("Example").size()), 2;

    auto userFromExample = db.getAllObjects("Example");
    ASSERT_EQ(u2.id, userFromExample.at(0).id);
    ASSERT_EQ(u2.age, userFromExample.at(0).age);
    ASSERT_EQ(u2.name, userFromExample.at(0).name);

    ASSERT_EQ(u3.id, userFromExample.at(1).id);
    ASSERT_EQ(u3.age, userFromExample.at(1).age);
    ASSERT_EQ(u3.name, userFromExample.at(1).name);
}

TEST_F(SQLiteDatabaseTest, getObjectsCount)
{
    db.createTable("Users");
    User u1{1, "Sam", 25};
    User u2{2, "Bob", 26};
    User u3{3, "Anna", 21};

    db.insert("Users", u1);
    db.insert("Users", u2);
    db.insert("Users", u3);
    ASSERT_EQ(db.getObjectsCount("Users"), 3);
    db.removeAllObjects("Users");
    ASSERT_EQ(db.getObjectsCount("Users"), 0);
}
TEST_F(SQLiteDatabaseTest, swapObjects)
{
    db.createTable("Users");
    User u1{1, "Sam", 25};
    User u2{2, "Bob", 26};
    User u3{3, "Anna", 21};

    db.insert("Users", u1);
    db.insert("Users", u2);
    db.insert("Users", u3);
    db.swapObjects("Users", "id=1", "id=3");

    ASSERT_EQ(db.getAllObjects("Users").at(0).name, u3.name);

    ASSERT_EQ(db.getAllObjects("Users").at(2).name, u1.name);
}

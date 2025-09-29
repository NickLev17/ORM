#include <iostream>
#include "../include/Database.h"
#include "../include/MySQLDatabase.h"
#include "../include/SQLiteDatabase.h"

using namespace std;

int main()
{

    // Create Objects
    string name[10]{"Alex", "Anna", "Peter", "Bob", "Olga", "Andrew", "Irina", "Georg", "Elena", "Samanta"};
    int age[10]{24, 25, 30, 40, 38, 24, 26, 27, 27, 42};
    vector<User> users;

    for (int i = 0; i < 10; i++)
    {

        User tmp{i + 1, name[i], age[i]};
        users.push_back(tmp);
    }

    // Class MySQLDatabase methods view
    {
        cout << "Class MySQLDatabase methods view:\n";
        Database *mysql_database = new MySQLDatabase("root", "simple1");

        mysql_database->createDatabase("Factory");

        // Creating other databse / Using database / Dropping database
        mysql_database->createDatabase("Example");
        mysql_database->useDatabase("Example");
        mysql_database->dropDatabase("Example");

        // Using database /Creating table
        mysql_database->useDatabase("Factory");
        mysql_database->createTable("Employee");

        // Exists table
        if (mysql_database->tableExists("Employee"))
            std::cout << "Table Employee exists \n";

        // Renaming table
        mysql_database->renameTable("Employee", "Workers");
        mysql_database->createTable("GoodWorkers");
        mysql_database->createTable("BadWorkers");

        // Getting name of the tables from database
        auto tables = mysql_database->getTables();

        for (const auto &t : tables)
            cout << "Table: " << t << "\n";

        // Dropping table
        mysql_database->dropTable("BadWorkers");

        // Inserting data in table
        for (const auto &user : users)
            mysql_database->insert("Workers", user);

        // Return Objects due to condition
        auto group_1 = mysql_database->getObjects("Workers", "name='Alex'");
        auto group_2 = mysql_database->getObjects("Workers", "age>35");

        for (const auto &g : group_1)
            cout << "Worker: " << g.name << " age: " << g.age << "\n";

        for (const auto &g : group_2)
            cout << "Worker: " << g.name << " age: " << g.age << "\n";

        // Return all Objects from table
        auto allWorkers = mysql_database->getAllObjects("Workers");
        for (const auto &w : allWorkers)
            cout << "Worker: " << w.name << " age: " << w.age << "\n";

        // Remove Object from table due to condition
        mysql_database->removeObject("Workers", "age>40");

        // Updating data in table
        User temp{1, "Tim", 32};
        mysql_database->update("Workers", temp, "id=1");

        // Order objects  by one field
        auto mysql_orderOneField = mysql_database->getObjectsOrderBy("Workers", "age");
        for (const auto &orderOneField : mysql_orderOneField)
            cout << "id: " << orderOneField.id << " " << orderOneField.name << " age: " << orderOneField.age << "\n";

        // Order  objects by two field
        auto mysql_orderTwoField = mysql_database->getObjectsOrderBy("Workers", "age", "name");
        for (const auto &orderTwoField : mysql_orderTwoField)
            cout << "id: " << orderTwoField.id << " " << orderTwoField.name << " age: " << orderTwoField.age << "\n";

        // Copying data from table1 to table2
        cout << "Copying Ttable:\n";
        mysql_database->copyAllObjectsInOtherTable("Workers", "GoodWorkers");

        auto goodWorkers = mysql_database->getAllObjects("GoodWorkers");

        for (const auto &w : goodWorkers)
            cout << "id: " << w.id << " " << w.name << " age: " << w.age << "\n";

        // Removeing all  objects from table
        mysql_database->removeAllObjects("GoodWorkers");

        // Copying data from table duo to condition + commit
        mysql_database->copyObjectsInOtherTable("Workers", "GoodWorkers", "age>26 && age<40");

        auto bestWorkers = mysql_database->getAllObjects("GoodWorkers");

        for (const auto &w : bestWorkers)
            cout << "id: " << w.id << " " << w.name << " age: " << w.age << "\n";

        // Start transaction + Rollback
        mysql_database->beginTransaction();
        bool result = mysql_database->execute("DROP TABLE GoodWorkers");
        if (result)
        {
            cout << "Comand Drop Table seccusfully\n";
        }
        else
        {
            cout << "Comand Drop Table unseccusfully\n";
        }
        mysql_database->rollback();

        // Getting objects count from the table
        cout << "Objects count : " << mysql_database->getObjectsCount("Workers") << "\n";

        // Swapping objects
        mysql_database->swapObjects("Workers", "id=1", "id=8");
    }
    cout << "--------------------------------------------------------\n";

    // Class SQLiteDatabase methods view
    {
        cout << "Class SQLiteDatabase methods view:\n";
        Database *sqlite_database = new SQLiteDatabase();

        sqlite_database->createDatabase("Factory");

        // Creating other databse / Using database / Dropping database
        sqlite_database->createDatabase("Example");
        sqlite_database->useDatabase("Example");
        sqlite_database->dropDatabase("Example");

        // Using database /Creating table
        sqlite_database->useDatabase("Factory");
        sqlite_database->createTable("Employee");

        // Exists table
        if (sqlite_database->tableExists("Employee"))
            std::cout << "Table Employee exists \n";

        // Renaming table
        sqlite_database->renameTable("Employee", "Workers");
        sqlite_database->createTable("GoodWorkers");
        sqlite_database->createTable("BadWorkers");

        // Getting name of the tables from database
        auto tables = sqlite_database->getTables();

        for (const auto &t : tables)
            cout << "Table: " << t << "\n";

        // Dropping table
        sqlite_database->dropTable("BadWorkers");

        // Inserting data in table
        for (const auto &user : users)
            sqlite_database->insert("Workers", user);

        // Return Objects due to condition
        auto group_1 = sqlite_database->getObjects("Workers", "name='Alex'");
        auto group_2 = sqlite_database->getObjects("Workers", "age>35");

        for (const auto &g : group_1)
            cout << "Worker: " << g.name << " age: " << g.age << "\n";

        for (const auto &g : group_2)
            cout << "Worker: " << g.name << " age: " << g.age << "\n";

        // Return all Objects from table
        auto allWorkers = sqlite_database->getAllObjects("Workers");
        for (const auto &w : allWorkers)
            cout << "Worker: " << w.name << " age: " << w.age << "\n";

        // Remove Object from table due to condition
        sqlite_database->removeObject("Workers", "age>40");

        // Updating data in table
        User temp{1, "Tim", 32};
        sqlite_database->update("Workers", temp, "id=1");

        // Order objects  by one field
        auto sqlite_orderOneField = sqlite_database->getObjectsOrderBy("Workers", "age");
        for (const auto &orderOneField : sqlite_orderOneField)
            cout << "id: " << orderOneField.id << " " << orderOneField.name << " age: " << orderOneField.age << "\n";

        // Order  objects by two field
        auto mysql_orderTwoField = sqlite_database->getObjectsOrderBy("Workers", "age", "name");
        for (const auto &orderTwoField : mysql_orderTwoField)
            cout << "id: " << orderTwoField.id << " " << orderTwoField.name << " age: " << orderTwoField.age << "\n";

        // Copying data from table1 to table2
        cout << "Copying table\n";
        sqlite_database->beginTransaction();
        sqlite_database->copyAllObjectsInOtherTable("Workers", "GoodWorkers");

        auto goodWorkers = sqlite_database->getAllObjects("GoodWorkers");

        for (const auto &w : goodWorkers)
            cout << "id: " << w.id << " " << w.name << " age: " << w.age << "\n";

        // Removeing all  objects from table
        sqlite_database->removeAllObjects("GoodWorkers");

        // Copying data from table duo to condition + commit
        sqlite_database->beginTransaction();
        sqlite_database->copyObjectsInOtherTable("Workers", "GoodWorkers", "age>26 AND age<40");

        auto bestWorkers = sqlite_database->getAllObjects("GoodWorkers");

        for (const auto &w : bestWorkers)
            cout << "id: " << w.id << " " << w.name << " age: " << w.age << "\n";

        // Start transaction + Rollback
        sqlite_database->beginTransaction();
        bool result = sqlite_database->execute("DROP TABLE GoodWorkers");
        if (result)
        {
            cout << "Comand Drop Table seccusfully\n";
        }
        else
        {
            cout << "Comand Drop Table unseccusfully\n";
        }
        sqlite_database->rollback();

        // Getting objects count from the table
        cout << "Objects count : " << sqlite_database->getObjectsCount("Workers") << "\n";

        // Swapping objects
        sqlite_database->swapObjects("Workers", "id=1", "id=8");
    }

    return 0;
}

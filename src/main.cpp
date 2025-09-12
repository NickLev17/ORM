#include <iostream>
#include "../include/Database.h"
#include "../include/MySQLDatabase.h"
#include "../include/SQLiteDatabase.h"

using namespace std;

int main()
{

    // Create Users
    string name[10]{"Alex", "Anna", "Piter", "Bob", "Olga", "Andrew", "Irina", "Georg", "Elena", "Samanta"};
    int age[10]{24, 25, 30, 40, 38, 24, 26, 27, 35, 42};
    vector<User> users;

    for (int i = 0; i < 10; i++)
    {

        // Database loading
        User tmp{i + 1, name[i], age[i]};
        users.push_back(tmp);
    }

    // Create databases
    Database *sqlite_database = new SQLiteDatabase();
    Database *mysql_database = new MySQLDatabase("root", "simple1");
    vector<Database *> databases{sqlite_database, mysql_database};

    for (const auto &db : databases)
    {
        db->createDatabase("List_of_users");
        db->useDatabase("List_of_users");
        db->createTable("Users");

        for (const auto &user : users)
            db->insert("Users", user);
    }

    // Show data in databases
    auto sqlite_order = databases.at(0)->getObjectsOrderBy("Users", "age");

    auto mysql_order = databases.at(1)->getObjectsOrderBy("Users", "age");
    databases.at(0)->beginTransaction();
    databases.at(1)->beginTransaction();

    cout << "SQLite database: \n";
    for (const auto &lite : sqlite_order)
        cout << "id: " << lite.id << " name: " << lite.name << " age: " << lite.age << "\n";

    cout << "MYSQL database: \n";
    for (const auto &base : mysql_order)
        cout << "id: " << base.id << " name: " << base.name << " age: " << base.age << "\n";

    return 0;
}

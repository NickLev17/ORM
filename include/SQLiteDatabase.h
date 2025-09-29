#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H
#include <iostream>
#include "Database.h"
#include <cstdio>
#include <sqlite3.h>
#include <string>
#include <vector>
#include <memory>
#include <gtest/gtest.h>
#include <stdexcept>

using namespace std;
/**
 * @brief class SQLiteDatabase
 * Представляет реализацию интерфейса Database для реализации ORM подхода при взаимодействии с базой данных SQLite.
 *
 */
class MYLIB_API SQLiteDatabase : public Database
{

public:
    /**
     * Указатель на объект sqlite3.
     */
    sqlite3 *db = nullptr;
    /**
     * Флаг соединения. При отсутствии соединения равен false, при наличии true.
     */
    bool connection = false;

    /**
     * @brief Конструктор класса.
     * Конструктор класса отвечает за корректное создание обьекта.
     */
    SQLiteDatabase();
    /**
     * @brief Деструктор класса.
     * Деструктор класса, отвечает за корректное уничтожение обьекта.
     *
     */
    ~SQLiteDatabase();
    /**
     * @brief Состояние соединения.
     * Метод отвечает за возврат состояния текущего соединения.
     * @return при наличии текущего соединения с БД возврщает true, в противном случае false.
     */
    bool isConnected() const override;
    /**
     * @brief Отключение от БД.
     * Метод отвечает за корректное завершение соединения с БД.
     *  */
    void disconnect() override;

    /**
     * @brief Установление соединения с БД.
     * Метод отвечает за покдлючение к БД.
     **/

    void connect(const string &, const string &) override;
    /**
     * @brief Открытие БД.
     * Метод отвечает за открытие выбранной базы данных.
     * @param db_file имя базы данных, виде константной ссылки на строку, с которой требуется взаимодействовать.
     *  */
    void openDatabase(const string &db_file) override;
    /**
     * @brief Создание БД.
     * Метод отвечает за создание базы данных.
     * @param db_file имя базы данных, которая будет создана.
     *  */
    void createDatabase(const string &db_file) override;
    /**
     * @brief Выбор БД.
     * Метод отвечает за выбор базы данных.
     * @param db_file имя базы данных, с которой требуется взаимодействовать.
     *  */
    void useDatabase(const string &db_file) override;
    /**
     * @brief Удаление БД.
     * Метод отвечает за удаление базы данных.
     * @param принимаети имя базы данных, в виде константной ссылки на строку, которую требуется удалить.
     *  */
    void dropDatabase(const string &db_file) override;
    /**
     * @brief Проверка наличия таблицы в БД.
     * Метод отвечает за проверку наличия таблицы в базе данных.
     * @param tableName имя таблицы в виде константной ссылки на строку.
     * @return при наличии таблицы в базе данных возврщает true, в противном случае false.
     *  */
    bool tableExists(const string &tableName) const override;
    /**
     * @brief Удаление таблицы
     * Метод отвечает за удаление таблцы из базе даных.
     * @param tableName имя таблицы, в виде константной ссылки на строку, которую требуется удалить из базы данных.
     *  */
    void dropTable(const string &tableName) override;
    /**
     * @brief Создание таблицы
     * Метод отвечает за создание таблцы в базе даных.
     * @param tableName имя будущей таблицы в виде константной ссылки на строку.
     *  */
    void createTable(const string &tableName) override;
    /**
     * @brief Переименование таблицы
     * Метод отвечает за переименование таблцы из базе даных.
     * @param tableName имя таблицы, в виде константной ссылки на строку, которую требуется переименовать.
     * @param newName новое имя, в виде константной ссылки на строку, которым будет переименована текущая таблица.
     *  */
    void renameTable(const string &tableName, const string &newName) override;
    /**
     * @brief Получаение вектора имен таблиц.
     * Метод возвращает вектор с названиями таблиц из текущей базы данных.
     * @return возвращает вектор vector<std::string> с именами таблицы.
     *  */
    vector<std::string> getTables() const override;
    /**
     * @brief Вставка обьектов в таблицу.
     * Метод отвечает вставку обьектов в таблицу базы данных.
     * @param tableName имя таблицы.
     * @param obj обьект ожидающий вставки в таблицу.
     *  */
    void insert(const std::string &tableName, const User &obj) override;
    /**
     * @brief Выбор элементов из таблицы по запросу.
     * Метод отвечает возврат элементов из таблицы в соответствии с определенным запросом.
     * @param query запрос для БД.
     * @return std::vector<User> вектор данных из таблицы.
     *  */
    std::vector<User> select(const std::string &query) override;
    /**
     * @brief Изменение обьекта по условию
     * Метод отвечает за изменение обьекта, который соответсвует определенному условию.
     * @param tableName имя таблицы из которой будут удалены обьекты.
     * @param ogj  обьект которым будет заменен обьект из таблицы.
     * @param condition условие, соответсвующему который обьект, будет изменен.
     *  */
    void update(const std::string &tableName, const User &obj, const std::string &condition) override;
    /**
     * @brief Удаление обьектов по условию
     * Метод отвечает за удаление обьектов соответствующих определенному условию.
     * @param tableName имя таблицы из которой будут удалены обьекты.
     * @param condition условие по которому будет осуществляться удаление обьектов.
     *  */
    void removeObject(const std::string &tableName, const std::string &condition) override;
    /**
     * @brief Вовзрат всех обьектов из таблицы.
     * Метод отвечает за возврат всех обьектов из таблицы.
     * @param tableName имя таблицы из которой будут возвращены обьекты.
     * @return std::vector<User> вектор обьектов из таблицы.
     *  */
    std::vector<User> getAllObjects(const std::string &tableName) const override;
    /**
     * @brief Вовзрат обьектов из таблицы в соответствии с определенным условием.
     * Метод отвечает за возврат обьектов, которые соответсвуют определенному условию.
     * @param tableName имя таблицы из которой будут возвращены обьекты.
     * @param condition условие, в соответсвии с которым будет сформирован вектор возврщаемых обьектов.
     * @return std::vector<User> вектор обьектов  сформированных по определенному условию.
     *  */
    std::vector<User> getObjects(const std::string &tableName, const string &condition) const override;
    /**
     * @brief Удаление всех обьектов из таблицы.
     * Метод отвечает за удаление всех обьектов из таблицы.
     * @param tableName имя таблицы из которой будут удалены все обьекты.
     *  */
    void removeAllObjects(const std::string &tableName) override;
    /**
     * @brief Получение сортированных по одному полю обьектов из таблицы
     * Метод отвечает за сортировку данных по одному выбранному полю.
     * @param tableName имя таблицы.
     * @param field  имя поля обьекта, по которому будет производится сортировка.
     *  */
    std::vector<User> getObjectsOrderBy(const std::string &tableName, const string &field) const override;
    /**
     * @brief Получение сортированных по двум полям обьектов из таблицы
     * Метод отвечает за сортировку данных по двум выбранному полю.
     * @param tableName имя таблицы.
     * @param field1  имя поля обьекта, по которому первым будет производится сортировка.
     * @param field2  имя поля обьекта, по которому во вторую очередь будет производится сортировка.
     *  */
    std::vector<User> getObjectsOrderBy(const std::string &tableName, const string &field1, const string &field2) const override;
    /**
     * @brief Копирование всех обьектоов из таблицы.
     * Метод отвечает за копирование всех обьектов из одной базы данных в другю.
     * @param tableNameFrom имя таблицы обьекты которой будут скопировны.
     * @param tableNameTo имя таблицы куда будут скопированы обьекты.
     *  */
    void copyAllObjectsInOtherTable(const std::string &tableNameFrom, const std::string &tableNameTo) override;
    /**
     * @brief Копирование обьектов из таблицы соответствующих условию.
     * Метод отвечает за копирование обьектов соответствующих определенному условию, из одной таблицы в другую.
     * @param tableNameFrom имя таблицы обьекты которой будут скопировны.
     * @param tableNameTo имя таблицы куда будут скопированы обьекты.
     * @param condition условие по которому будет осуществляться копирование обьектов.
     *  */
    void copyObjectsInOtherTable(const std::string &tableNameFrom, const std::string &tableNameTo, const std::string &condition) override;
    /**
     * @brief Начало транзакции.
     * Метод устанавливает начало транзакции.
     */
    void beginTransaction() override;
    /**
     * @brief Коммит.
     * Метод применяет текущие изменения в базе данных.
     */
    void commit() override;
    /**
     * @brief Отмена транзакции.
     * Метод отменяет текущие изменения в базе данных.
     */
    void rollback() override;
    /**
     * @brief Закрытие базы данных
     * Метод отвечает за корректное отключение от базы данных и последующее её закрытие.
     */
    void close();
    /**
     * @brief Простой запрос
     * Метод отвечает за реализацию простых запросов к БД.
     * @param sql запрос в виде константной ссылки на строку.
     * @return при успешном проведении запроса возврщает true, в противном случае false.
     *  */
    bool execute(const string &sql) override;
    /**
     * @brief Получение количества обьектов в таблице.
     * Метод отвечает за возврат количеств обьектов в таблице.
     * @param tableName имя таблицы.
     * @return количеств пользователей в таблице.
     *  */
    int getObjectsCount(const std::string &tableName) const override;
    /**
     * @brief Обмен запсей в таблице между собой.
     * Метод отвечает за обмен записей в таблице.
     * @param tableName имя таблицы.
     * @param id_obj id первого обьекта, который будет заменен вторым обьектом.
     * @param id_other_obj id второго обьекта, который будет заменен первым обьектом.
     *  */
    void swapObjects(const std::string &tableName, const std::string &id_obj, const std::string &id_other_obj) override;

private:
    FRIEND_TEST(SQLiteDatabaseTest, CreateDatabaseInMemory);
};

#endif

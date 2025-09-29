#ifndef DATABASE_H
#define DATABASE_H

#ifdef _WIN32
#ifdef MYLIBRARY_EXPORTS
#define MYLIB_API __declspec(dllexport)
#else
#define MYLIB_API __declspec(dllimport)
#endif
#else
#define MYLIB_API
#endif

#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
 * @brief Структура User.
 *
 * Характеризует тип данных, с которым взаимодействует библиотека  Database.so.
 */
struct User
{
    int id;           /**< идентификационный номер пользователя в таблице */
    std::string name; /**< имя пользователя */
    int age;          /**< возраст пользователя */
};

/**
 * @brief class Database
 * Представляет собой интерфейс для работы с базой данных.
 *
 */
class MYLIB_API Database
{

public:
    /**
     * @brief Конструктор класса.
     * Виртуальный конструктор класса, отвечает за создание обьекта.
     *
     */
    Database() {};
    /**
     * @brief Деструктор класса.
     * Виртуальный деструктор класса, отвечает за корректное уничтожение обьекта.
     *
     */
    virtual ~Database() = default;

    /**
     * @brief Конструктор копирования класса.
     * Конструктор копирования класса -отсутствует.
     *  */
    Database(const Database &) = delete;
    /**
     * @brief Оператор присваивания.
     * Оператор присваивния класса -отсутствует.
     *  */
    Database &operator=(const Database &) = delete;

    /**
     * @brief Простой запрос
     * Метод отвечает за реализацию простых запросов к БД.
     * @param sql запрос в виде константной ссылки на строку.
     * @return при успешном проведении запроса возврщает true, в противном случае false.
     *  */
    virtual bool execute(const string &sql) = 0;

    /**
     * @brief Установление соединения с БД.
     * Метод отвечает за покдлючение к БД.
     * @param name имя пользователя в виде константной строки.
     * @param password пароль в виде константной ссылки на строку.
     *  */
    virtual void connect(const string &name, const string &password) = 0;

    /**
     * @brief Состояние соединения.
     * Метод отвечает за возврат состояния текущего соединения.
     * @return при наличии текущего соединения с БД возврщает true, в противном случае false.
     *  */
    virtual bool isConnected() const = 0;

    /**
     * @brief Отключение от БД.
     * Метод отвечает за корректное завершение соединения с БД.
     *  */
    virtual void disconnect() = 0;

    // Work database

    /**
     * @brief Удаление БД.
     * Метод отвечает за удаление базы данных.
     * @param принимаети имя базы данных, в виде константной ссылки на строку, которую требуется удалить.
     *  */
    virtual void dropDatabase(const string &db_file) = 0;

    /**
     * @brief Открытие БД.
     * Метод отвечает за открытие выбранной базы данных.
     * @param db_file имя базы данных, виде константной ссылки на строку, с которой требуется взаимодействовать.
     *  */
    virtual void openDatabase(const string &db_file) = 0;

    /**
     * @brief Создание БД.
     * Метод отвечает за создание базы данных.
     * @param db_file имя базы данных, которая будет создана.
     *  */
    virtual void createDatabase(const string &db_file) = 0;

    /**
     * @brief Выбор БД.
     * Метод отвечает за выбор базы данных.
     * @param db_file имя базы данных, с которой требуется взаимодействовать.
     *  */
    virtual void useDatabase(const string &db_file) = 0;

    // Work table
    /**
     * @brief Проверка наличия таблицы в БД.
     * Метод отвечает за проверку наличия таблицы в базе данных.
     * @param tableName имя таблицы в виде константной ссылки на строку.
     * @return при наличии таблицы в базе данных возврщает true, в противном случае false.
     *  */
    virtual bool tableExists(const string &tableName) const = 0;

    /**
     * @brief Создание таблицы
     * Метод отвечает за создание таблцы в базе даных.
     * @param tableName имя будущей таблицы в виде константной ссылки на строку.
     *  */
    virtual void createTable(const string &tableName) = 0;
    /**
     * @brief Удаление таблицы
     * Метод отвечает за удаление таблцы из базе даных.
     * @param tableName имя таблицы, в виде константной ссылки на строку, которую требуется удалить из базы данных.
     *  */
    virtual void dropTable(const string &tableName) = 0;
    /**
     * @brief Переименование таблицы
     * Метод отвечает за переименование таблцы из базе даных.
     * @param tableName имя таблицы, в виде константной ссылки на строку, которую требуется переименовать.
     * @param newName новое имя, в виде константной ссылки на строку, которым будет переименована текущая таблица.
     *  */
    virtual void renameTable(const string &tableName, const string &newName) = 0;
    /**
     * @brief Получаение вектора имен таблиц.
     * Метод возвращает вектор с названиями таблиц из текущей базы данных.
     * @return возвращает вектор vector<std::string> с именами таблицы.
     *  */
    virtual vector<std::string> getTables() const = 0;

    // Mapping data (CRUD, ORDER,COPY)
    /**
     * @brief Вставка обьектов в таблицу.
     * Метод отвечает вставку обьектов в таблицу базы данных.
     * @param tableName имя таблицы.
     * @param obj обьект ожидающий вставки в таблицу.
     *  */
    virtual void insert(const std::string &tableName, const User &obj) = 0;
    /**
     * @brief Выбор элементов из таблицы по запросу.
     * Метод отвечает возврат элементов из таблицы в соответствии с определенным запросом.
     * @param query запрос для БД.
     * @return std::vector<User> вектор данных из таблицы.
     *  */
    virtual std::vector<User> select(const std::string &query) = 0;
    /**
     * @brief Вовзрат всех обьектов из таблицы.
     * Метод отвечает за возврат всех обьектов из таблицы.
     * @param tableName имя таблицы из которой будут возвращены обьекты.
     * @return std::vector<User> вектор обьектов из таблицы.
     *  */
    virtual std::vector<User> getAllObjects(const std::string &tableName) const = 0;
    /**
     * @brief Вовзрат обьектов из таблицы в соответствии с определенным условием.
     * Метод отвечает за возврат обьектов, которые соответсвуют определенному условию.
     * @param tableName имя таблицы из которой будут возвращены обьекты.
     * @param condition условие, в соответсвии с которым будет сформирован вектор возврщаемых обьектов.
     * @return std::vector<User> вектор обьектов  сформированных по определенному условию.
     *  */
    virtual std::vector<User> getObjects(const std::string &tableName, const string &condition) const = 0;
    /**
     * @brief Изменение обьекта по условию
     * Метод отвечает за изменение обьекта, который соответсвует определенному условию.
     * @param tableName имя таблицы из которой будут удалены обьекты.
     * @param ogj  обьект которым будет заменен обьект из таблицы.
     * @param condition условие, соответсвующему который обьект, будет изменен.
     *  */
    virtual void update(const std::string &tableName, const User &obj, const std::string &condition) = 0;
    /**
     * @brief Удаление обьектов по условию
     * Метод отвечает за удаление обьектов соответствующих определенному условию.
     * @param tableName имя таблицы из которой будут удалены обьекты.
     * @param condition условие по которому будет осуществляться удаление обьектов.
     *  */
    virtual void removeObject(const std::string &tableName, const std::string &condition) = 0;
    /**
     * @brief Удаление всех обьектов из таблицы.
     * Метод отвечает за удаление всех обьектов из таблицы.
     * @param tableName имя таблицы из которой будут удалены все обьекты.
     *  */
    virtual void removeAllObjects(const std::string &tableName) = 0;

    /**
     * @brief Копирование всех обьектоов из таблицы.
     * Метод отвечает за копирование всех обьектов из одной базы данных в другю.
     * @param tableNameFrom имя таблицы обьекты которой будут скопировны.
     * @param tableNameTo имя таблицы куда будут скопированы обьекты.
     *  */
    virtual void copyAllObjectsInOtherTable(const std::string &tableNameFrom, const std::string &tableNameTo) = 0;
    /**
     * @brief Копирование обьектов из таблицы соответствующих условию.
     * Метод отвечает за копирование обьектов соответствующих определенному условию, из одной таблицы в другую.
     * @param tableNameFrom имя таблицы обьекты которой будут скопировны.
     * @param tableNameTo имя таблицы куда будут скопированы обьекты.
     * @param condition условие по которому будет осуществляться копирование обьектов.
     *  */
    virtual void copyObjectsInOtherTable(const std::string &tableNameFrom, const std::string &tableNameTo, const std::string &condition) = 0;

    // Return object/objects whis current condition
    /**
     * @brief Получение сортированных по одному полю обьектов из таблицы
     * Метод отвечает за сортировку данных по одному выбранному полю.
     * @param tableName имя таблицы.
     * @param field  имя поля обьекта, по которому будет производится сортировка.
     *  */
    virtual std::vector<User> getObjectsOrderBy(const std::string &tableName, const string &field) const = 0;
    /**
     * @brief Получение сортированных по двум полям обьектов из таблицы
     * Метод отвечает за сортировку данных по двум выбранному полю.
     * @param tableName имя таблицы.
     * @param field1  имя поля обьекта, по которому первым будет производится сортировка.
     * @param field2  имя поля обьекта, по которому во вторую очередь будет производится сортировка.
     *  */
    virtual std::vector<User> getObjectsOrderBy(const std::string &tableName, const string &field1, const string &field2) const = 0;

    /**
     * @brief Получение количества обьектов в таблице.
     * Метод отвечает за возврат количеств обьектов в таблице.
     * @param tableName имя таблицы.
     * @return количеств пользователей в таблице.
     *  */
    virtual int getObjectsCount(const std::string &tableName) const = 0;
    /**
     * @brief Обмен запсей в таблице между собой.
     * Метод отвечает за обмен записей в таблице.
     * @param tableName имя таблицы.
     * @param id_obj id первого обьекта, который будет заменен вторым обьектом.
     * @param id_other_obj id второго обьекта, который будет заменен первым обьектом.
     *  */
    virtual void swapObjects(const std::string &tableName, const std::string &id_obj, const std::string &id_other_obj) = 0;

    //  Client desides
    /**
     * @brief Начало транзакции.
     * Метод устанавливает начало транзакции.
     */
    virtual void beginTransaction() = 0;
    /**
     * @brief Коммит.
     * Метод применяет текущие изменения в базе данных.
     */
    virtual void commit() = 0;
    /**
     * @brief Отмена транзакции.
     * Метод отменяет текущие изменения в базе данных.
     */
    virtual void rollback() = 0;
};

#endif

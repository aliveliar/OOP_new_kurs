#include "database.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>

// Конструктор класса Database с настройкой подключения к PostgreSQL
Database::Database(QObject *parent) : QObject(parent)
{
    m_db = new QSqlDatabase(QSqlDatabase::addDatabase("QPSQL", "phonebook_db"));
    m_db->setHostName("localhost");
    m_db->setPort(5432);
    m_db->setDatabaseName("pgs");
    m_db->setUserName("postgres");
    m_db->setPassword("1234");
}

Database::~Database()
{
    disconnect();
    if (m_db) {
        delete m_db;
        m_db = nullptr;
    }
}

// Подключение к базе данных PostgreSQL
bool Database::connect_to_database()
{
    if (!m_db->open()) {
        qDebug() << "PostgreSQL connection error:" << m_db->lastError().text();
        return false;
    }

    qDebug() << "Connected to PostgreSQL successfully!";
    create_table();
    return true;
}

// Отключение от базы данных
bool Database::disconnect()
{
    if (is_connected()) {
        m_db->close();
        qDebug() << "Database disconnected";
        return true;
    }
    return false;
}

// Создание таблицы contacts_test в базе данных
bool Database::create_table()
{
    QSqlQuery query(*m_db);
    bool result = query.exec(
        "CREATE TABLE IF NOT EXISTS contacts_test ("
        "id SERIAL PRIMARY KEY,"
        "first_name TEXT NOT NULL,"
        "surname TEXT NOT NULL,"
        "middle_name TEXT,"
        "address TEXT,"
        "birthday TEXT,"
        "email TEXT UNIQUE NOT NULL,"
        "phones TEXT NOT NULL)"
        );

    if (result) {
        qDebug() << "Table contacts_test created/checked";
    } else {
        qDebug() << "Table creation error:" << query.lastError().text();
    }

    return result;
}

// Сохранение всех контактов в базу данных
bool Database::save_contacts(const QList<Contact>& contacts)
{
    if (!is_connected()) {
        qDebug() << "Database not connected";
        return false;
    }

    QSqlQuery clearQuery(*m_db);
    if (!clearQuery.exec("DELETE FROM contacts_test")) {
        qDebug() << "Table clear error:" << clearQuery.lastError().text();
        return false;
    }

    QString sql = "INSERT INTO contacts_test (first_name, surname, middle_name, "
                  "address, birthday, email, phones) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?)";

    QSqlQuery query(*m_db);
    query.prepare(sql);

    for (const Contact& contact : contacts) {
        query.addBindValue(contact.get_firstname());
        query.addBindValue(contact.get_surname());
        query.addBindValue(contact.get_middlename());
        query.addBindValue(contact.get_address());
        query.addBindValue(contact.get_birthday());

        query.addBindValue(contact.get_email());
        query.addBindValue(phones_to_string(contact.get_phones()));

        if (!query.exec()) {
            qDebug() << "Contact save error:" << query.lastError().text();
            return false;
        }
    }

    qDebug() << "Data saved to database. Contacts:" << contacts.size();
    return true;
}

// Загрузка всех контактов из базы данных
QList<Contact> Database::load_contacts()
{
    QList<Contact> contacts;

    if (!is_connected()) {
        qDebug() << "Database not connected";
        return contacts;
    }

    QSqlQuery query("SELECT id, first_name, surname, middle_name, "
                    "address, birthday, email, phones "
                    "FROM contacts_test", *m_db);

    while (query.next()) {
        contacts.append(contact_from_query(query));
    }

    qDebug() << "Contacts loaded from database:" << contacts.size();
    return contacts;
}

// Добавление одного контакта в базу данных
bool Database::add_contact(const Contact& contact)
{
    if (!is_connected()) return false;

    QSqlQuery query(*m_db);
    query.prepare("INSERT INTO contacts_test (first_name, surname, middle_name, "
                  "address, birthday, email, phones) "
                  "VALUES (:first_name, :surname, :middle_name, :address, "
                  ":birthday, :email, :phones)");

    query.bindValue(":first_name", contact.get_firstname());
    query.bindValue(":surname", contact.get_surname());
    query.bindValue(":middle_name", contact.get_middlename());
    query.bindValue(":address", contact.get_address());

    QDate birthday = parse_birthday(contact.get_birthday());
    query.bindValue(":birthday", birthday.isValid() ? birthday : QVariant());

    query.bindValue(":email", contact.get_email());
    query.bindValue(":phones", phones_to_string(contact.get_phones()));

    if (query.exec()) {
        qDebug() << "Contact added to database";
        return true;
    } else {
        qDebug() << "Contact add error:" << query.lastError().text();
        return false;
    }
}

// Создание объекта Contact из результата SQL-запроса
Contact Database::contact_from_query(const QSqlQuery& query)
{
    Contact contact;

    contact.set_firstname(query.value("first_name").toString());
    contact.set_surname(query.value("surname").toString());
    contact.set_middlename(query.value("middle_name").toString());
    contact.set_address(query.value("address").toString());
    contact.set_birthday(query.value("birthday").toString());

    contact.set_email(query.value("email").toString());

    QString phonesStr = query.value("phones").toString();
    QStringList phones = string_to_phones(phonesStr);
    contact.set_phones(phones);

    return contact;
}

// Преобразование списка телефонов в строку с разделителем '|'
QString Database::phones_to_string(const QStringList& phones)
{
    return phones.join("|");
}

// Преобразование строки с разделителем '|' в список телефонов
QStringList Database::string_to_phones(const QString& str)
{
    return str.split("|", Qt::SkipEmptyParts);
}

// Парсинг строки даты рождения в формате "DD-MM-YYYY"
QDate Database::parse_birthday(const QString& birthdayStr)
{
    if (birthdayStr.isEmpty()){
        return QDate();
    }
    return QDate::fromString(birthdayStr, "DD-MM-YYYY");
}

#pragma once

#include <QObject>
#include <QtSql>
#include "contacts.h"

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();

    bool connect_to_database();
    bool disconnect();
    bool is_connected() const { return m_db && m_db->isOpen(); }

    bool create_table();
    bool save_contacts(const QList<Contact>& contacts);
    QList<Contact> load_contacts();

    bool add_contact(const Contact& contact);
    bool update_contact(int id, const Contact& contact);
    bool delete_contact(int id);
    bool is_email_unique(const QString& email, int excludeId = -1);

    int get_contact_id_by_email(const QString& email);

private:
    QSqlDatabase* m_db = nullptr;

    Contact contact_from_query(const QSqlQuery& query);
    QString phones_to_string(const QStringList& phones);
    QStringList string_to_phones(const QString& str);
    QDate parse_birthday(const QString& birthdayStr);
};

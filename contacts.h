#pragma once
#include <QString>
#include <QStringList>
#include <QDate>

class Contact {
public:
    Contact(QString firstname = "", QString surname = "", QString email = "", QStringList phones = {}, QString middlename = "", QString address = "", QString birthday = "");
    Contact(const Contact& other);
    ~Contact();

    Contact& operator=(const Contact& other);
    bool operator==(const Contact& other) const;

    QString get_firstname() const { return m_firstname; }
    QString get_surname() const { return m_surname; }
    QString get_middlename() const { return m_middlename; }
    QString get_address() const { return m_address; }
    QString get_birthday() const { return m_birthday; }
    QString get_email() const { return m_email; }
    QStringList get_phones() const { return m_phones; }
    int get_phones_count() const { return m_phones.size(); }

    bool set_firstname(const QString& firstname);
    bool set_surname(const QString& surname);
    bool set_middlename(const QString& middlename);
    bool set_address(const QString& address);
    bool set_birthday(const QString& birthday);
    bool set_email(const QString& email);
    bool set_phones(const QStringList& phones);

    bool add_phone(const QString& phone);
    bool remove_phone(int index);

    bool is_valid() const;
    bool is_valid_for_edit() const;
    QString validate_for_edit() const;
    bool valid_name(const QString& name) const;
    bool valid_email(const QString& email) const;
    bool valid_phone(const QString& phone) const;
    bool valid_birthday(const QString& birthday, bool allowEmpty = true) const;
    bool contains_firstname_in_email(const QString& email) const;

    QString normalize_phone(const QString& phone) const;
    QString trim(const QString& str) const;

    QString get_smallest_phone() const;
    QString to_string() const;

    void spaces_email(QString& email) const;

private:
    QString m_firstname;
    QString m_surname;
    QString m_middlename;
    QString m_address;
    QString m_birthday;
    QString m_email;
    QStringList m_phones;
};

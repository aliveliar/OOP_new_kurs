#pragma once
#include "contacts.h"
#include <QObject>
#include <QList>
#include <QString>

class Contactbook : public QObject {
    Q_OBJECT

public:
    Contactbook(QObject* parent = nullptr);
    ~Contactbook();

    bool add_contact(const Contact& contact);
    bool remove_contact(int index);
    bool update_contact(int index, const Contact& contact);
    Contact get_contact(int index) const;
    QList<Contact> get_all_contacts() const { return m_contacts; }
    int contact_count() const { return m_contacts.size(); }

    QList<Contact> search_by_firstname(const QList<Contact>& contacts, const QString& criteria) const;
    QList<Contact> search_by_surname(const QList<Contact>& contacts, const QString& criteria) const;
    QList<Contact> search_by_middlename(const QList<Contact>& contacts, const QString& criteria) const;
    QList<Contact> search_by_email(const QList<Contact>& contacts, const QString& criteria) const;
    QList<Contact> search_by_phone(const QList<Contact>& contacts, const QString& criteria) const;
    QList<Contact> search_by_address(const QList<Contact>& contacts, const QString& criteria) const;
    QList<Contact> search_by_birthday(const QList<Contact>& contacts, const QString& criteria) const;

    void sort_by_firstname();
    void sort_by_surname();
    void sort_by_middlename();
    void sort_by_email();
    void sort_by_birthday();
    void sort_by_address();
    void sort_by_phones();

    bool load_from_file(const QString& filename = "contacts.txt");
    bool save_to_file(const QString& filename = "contacts.txt");

signals:
    void contacts_changed();

private:
    QList<Contact> m_contacts;
};

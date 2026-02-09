#include "contactbook.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <algorithm>

Contactbook::Contactbook(QObject* parent) : QObject(parent) {}

Contactbook::~Contactbook() {}

bool Contactbook::add_contact(const Contact& contact) {
    if (contact.is_valid()) {
        m_contacts.append(contact);
        emit contacts_changed();
        return true;
    }
    return false;
}

bool Contactbook::remove_contact(int index) {
    if (index >= 0 && index < m_contacts.size()) {
        m_contacts.removeAt(index);
        emit contacts_changed();
        return true;
    }
    return false;
}

bool Contactbook::update_contact(int index, const Contact& contact) {
    if (index >= 0 && index < m_contacts.size() && contact.is_valid_for_edit()) {
        m_contacts[index] = contact;
        emit contacts_changed();
        return true;
    }
    return false;
}

Contact Contactbook::get_contact(int index) const {
    if (index >= 0 && index < m_contacts.size()) {
        return m_contacts[index];
    }
    return Contact();
}

QList<Contact> Contactbook::search_by_firstname(const QList<Contact>& contacts, const QString& criteria) const {
    QList<Contact> result;
    for (const Contact& contact : contacts) {
        if (contact.get_firstname().contains(criteria, Qt::CaseInsensitive)) {
            result.append(contact);
        }
    }
    return result;
}

QList<Contact> Contactbook::search_by_surname(const QList<Contact>& contacts, const QString& criteria) const {
    QList<Contact> result;
    for (const Contact& contact : contacts) {
        if (contact.get_surname().contains(criteria, Qt::CaseInsensitive)) {
            result.append(contact);
        }
    }
    return result;
}

QList<Contact> Contactbook::search_by_middlename(const QList<Contact>& contacts, const QString& criteria) const {
    QList<Contact> result;
    for (const Contact& contact : contacts) {
        if (contact.get_middlename().contains(criteria, Qt::CaseInsensitive)) {
            result.append(contact);
        }
    }
    return result;
}

QList<Contact> Contactbook::search_by_email(const QList<Contact>& contacts, const QString& criteria) const {
    QList<Contact> result;
    for (const Contact& contact : contacts) {
        if (contact.get_email().contains(criteria, Qt::CaseInsensitive)) {
            result.append(contact);
        }
    }
    return result;
}

QList<Contact> Contactbook::search_by_phone(const QList<Contact>& contacts, const QString& criteria) const {
    QList<Contact> result;
    for (const Contact& contact : contacts) {
        for (const QString& phone : contact.get_phones()) {
            if (phone.contains(criteria)) {
                result.append(contact);
                break;
            }
        }
    }
    return result;
}

QList<Contact> Contactbook::search_by_address(const QList<Contact>& contacts, const QString& criteria) const {
    QList<Contact> result;
    for (const Contact& contact : contacts) {
        if (contact.get_address().contains(criteria, Qt::CaseInsensitive)) {
            result.append(contact);
        }
    }
    return result;
}

QList<Contact> Contactbook::search_by_birthday(const QList<Contact>& contacts, const QString& criteria) const {
    QList<Contact> result;
    for (const Contact& contact : contacts) {
        if (contact.get_birthday().contains(criteria)) {
            result.append(contact);
        }
    }
    return result;
}

void Contactbook::sort_by_firstname() {
    std::sort(m_contacts.begin(), m_contacts.end(),
              [](const Contact& a, const Contact& b) { return a.get_firstname() < b.get_firstname(); });
    emit contacts_changed();
}

void Contactbook::sort_by_surname() {
    std::sort(m_contacts.begin(), m_contacts.end(),
              [](const Contact& a, const Contact& b) { return a.get_surname() < b.get_surname(); });
    emit contacts_changed();
}

void Contactbook::sort_by_middlename() {
    std::sort(m_contacts.begin(), m_contacts.end(),
              [](const Contact& a, const Contact& b) { return a.get_middlename() < b.get_middlename(); });
    emit contacts_changed();
}

void Contactbook::sort_by_email() {
    std::sort(m_contacts.begin(), m_contacts.end(),
              [](const Contact& a, const Contact& b) { return a.get_email() < b.get_email(); });
    emit contacts_changed();
}

void Contactbook::sort_by_birthday() {
    std::sort(m_contacts.begin(), m_contacts.end(),
              [](const Contact& a, const Contact& b) { return a.get_birthday() < b.get_birthday(); });
    emit contacts_changed();
}

void Contactbook::sort_by_address() {
    std::sort(m_contacts.begin(), m_contacts.end(),
              [](const Contact& a, const Contact& b) { return a.get_address() < b.get_address(); });
    emit contacts_changed();
}

void Contactbook::sort_by_phones() {
    std::sort(m_contacts.begin(), m_contacts.end(),
              [](const Contact& a, const Contact& b) {
                  QString aPhone = a.get_smallest_phone();
                  QString bPhone = b.get_smallest_phone();

                  if (aPhone.isEmpty() && bPhone.isEmpty()) return false;
                  if (aPhone.isEmpty()) return false;
                  if (bPhone.isEmpty()) return true;

                  return aPhone < bPhone;
              });
    emit contacts_changed();
}

// Загрузка контактов из текстового файла с пользовательским форматом
bool Contactbook::load_from_file(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    m_contacts.clear();
    QTextStream in(&file);
    QString line;
    while (!in.atEnd()) {
        line = in.readLine();
        if (line == "===CONTACT===") {
            QString firstname = in.readLine();
            QString surname = in.readLine();
            QString middlename = in.readLine();
            QString address = in.readLine();
            QString birthday = in.readLine();
            QString email = in.readLine();

            QStringList phones;
            while (!in.atEnd()) {
                line = in.readLine();
                if (line == "===END===") break;
                if (!line.isEmpty()) {
                    phones.append(line);
                }
            }

            Contact contact(firstname, surname, email, phones, middlename, address, birthday);
            m_contacts.append(contact);
        }
    }

    file.close();
    emit contacts_changed();
    return true;
}

// Сохранение контактов в текстовый файл с пользовательским форматом
bool Contactbook::save_to_file(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    for (const Contact& contact : m_contacts) {
        out << "===CONTACT===\n";
        out << contact.get_firstname() << "\n";
        out << contact.get_surname() << "\n";
        out << contact.get_middlename() << "\n";
        out << contact.get_address() << "\n";
        out << contact.get_birthday() << "\n";
        out << contact.get_email() << "\n";

        for (const QString& phone : contact.get_phones()) {
            out << phone << "\n";
        }
        out << "===END===\n";
    }

    file.flush();
    file.close();
    return true;
}

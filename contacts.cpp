#include "contacts.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDate>

Contact::Contact(QString firstname, QString surname, QString email, QStringList phones, QString middlename, QString address, QString birthday)
    : m_firstname(firstname), m_surname(surname), m_middlename(middlename), m_address(address), m_birthday(birthday), m_email(email), m_phones(phones) {}

Contact::Contact(const Contact& other) {
    *this = other;
}

Contact::~Contact() {}

Contact& Contact::operator=(const Contact& other) {
    if (this != &other) {
        m_firstname = other.m_firstname;
        m_surname = other.m_surname;
        m_middlename = other.m_middlename;
        m_address = other.m_address;
        m_birthday = other.m_birthday;
        m_email = other.m_email;
        m_phones = other.m_phones;
    }
    return *this;
}

bool Contact::operator==(const Contact& other) const {
    return m_firstname == other.m_firstname &&
           m_surname == other.m_surname &&
           m_middlename == other.m_middlename &&
           m_address == other.m_address &&
           m_birthday == other.m_birthday &&
           m_email == other.m_email &&
           m_phones == other.m_phones;
}

bool Contact::set_firstname(const QString& firstname) {
    if (valid_name(firstname)) {
        m_firstname = trim(firstname);
        return true;
    }
    return false;
}

bool Contact::set_surname(const QString& surname) {
    if (valid_name(surname)) {
        m_surname = trim(surname);
        return true;
    }
    return false;
}

bool Contact::set_middlename(const QString& middlename) {
    if (middlename.isEmpty() || valid_name(middlename)) {
        m_middlename = trim(middlename);
        return true;
    }
    return false;
}

bool Contact::set_address(const QString& address) {
    m_address = trim(address);
    return true;
}

bool Contact::set_birthday(const QString& birthday) {
    if (birthday.isEmpty() || valid_birthday(birthday, true)) {
        m_birthday = trim(birthday);
        return true;
    }
    return false;
}

bool Contact::set_email(const QString& email) {
    QString emailCopy = email;
    spaces_email(emailCopy);
    if (valid_email(emailCopy)) {
        m_email = trim(emailCopy);
        return true;
    }
    return false;
}

bool Contact::set_phones(const QStringList& phones) {
    for (const QString& phone : phones) {
        if (!valid_phone(phone)) return false;
    }
    m_phones.clear();
    for (const QString& phone : phones) {
        m_phones.append(normalize_phone(trim(phone)));
    }
    return true;
}

bool Contact::add_phone(const QString& phone) {
    if (valid_phone(phone)) {
        m_phones.append(normalize_phone(trim(phone)));
        return true;
    }
    return false;
}

bool Contact::remove_phone(int index) {
    if (index >= 0 && index < m_phones.size()) {
        m_phones.removeAt(index);
        return true;
    }
    return false;
}

bool Contact::is_valid() const {
    return !m_firstname.isEmpty() && !m_surname.isEmpty() && !m_email.isEmpty() && !m_phones.isEmpty();
}

bool Contact::is_valid_for_edit() const {
    return is_valid() &&
           valid_name(m_firstname) &&
           valid_name(m_surname) &&
           (m_middlename.isEmpty() || valid_name(m_middlename)) &&
           valid_email(m_email) &&
           (m_birthday.isEmpty() || valid_birthday(m_birthday, false)) &&
           !m_phones.isEmpty();
}

QString Contact::validate_for_edit() const {
    QStringList errors;

    if (m_firstname.isEmpty()) {
        errors << "First name is required";
    } else if (!valid_name(m_firstname)) {
        errors << "Invalid first name format";
    }

    if (m_surname.isEmpty()) {
        errors << "Surname is required";
    } else if (!valid_name(m_surname)) {
        errors << "Invalid surname format";
    }

    if (!m_middlename.isEmpty() && !valid_name(m_middlename)) {
        errors << "Invalid middle name format";
    }

    if (m_email.isEmpty()) {
        errors << "Email is required";
    } else if (!valid_email(m_email)) {
        errors << "Invalid email format";
    }

    if (m_phones.isEmpty()) {
        errors << "At least one phone number is required";
    } else {
        for (const QString& phone : m_phones) {
            if (!valid_phone(phone)) {
                errors << "Invalid phone number format";
                break;
            }
        }
    }

    if (!m_birthday.isEmpty() && !valid_birthday(m_birthday, false)) {
        errors << "Invalid birthday format or date is in the future";
    }

    return errors.join("\n");
}

bool Contact::valid_name(const QString& name) const {
    if (name.isEmpty()) return false;
    QRegularExpression regex("^[a-zA-Z][a-zA-Z0-9\\-]*[a-zA-Z0-9]$");
    return regex.match(trim(name)).hasMatch();
}

bool Contact::contains_firstname_in_email(const QString& email) const {
    if (m_firstname.isEmpty() || email.isEmpty()) return false;

    QString firstnameLower = m_firstname.toLower();
    QString emailLower = email.toLower();

    int atPos = emailLower.indexOf('@');
    if (atPos == -1) return false;

    QString localPart = emailLower.left(atPos);

    return localPart.contains(firstnameLower);
}

bool Contact::valid_email(const QString& email) const {
    QRegularExpression regex("^[a-zA-Z0-9]+@[a-zA-Z0-9]+\\.[a-zA-Z0-9]+$");
    if (!regex.match(trim(email)).hasMatch()) {
        return false;
    }

    return contains_firstname_in_email(email);
}

bool Contact::valid_phone(const QString& phone) const {
    QRegularExpression regex("^(\\+7|8)[\\s(]*(\\d{3})[\\s)]*(\\d{3})[\\s-]*(\\d{2})[\\s-]*(\\d{2})$");
    return regex.match(trim(phone)).hasMatch();
}

bool Contact::valid_birthday(const QString& birthday, bool allowEmpty) const {
    QString trimmed = trim(birthday);
    if (trimmed.isEmpty()) return allowEmpty;

    if (trimmed.length() != 10 || trimmed[2] != '-' || trimmed[5] != '-')
        return false;

    int day = trimmed.mid(0, 2).toInt();
    int month = trimmed.mid(3, 2).toInt();
    int year = trimmed.mid(6, 4).toInt();

    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    QDate date(year, month, day);
    if (!date.isValid()) return false;

    return date <= QDate::currentDate();
}

QString Contact::normalize_phone(const QString& phone) const {
    QString normalized;
    for (QChar ch : phone) {
        if (ch.isDigit() || ch == '+') normalized.append(ch);
    }

    if (normalized.length() == 11 && normalized[0] == '8') {
        normalized[0] = '7';
        normalized = '+' + normalized;
    } else if (normalized.length() == 10) {
        normalized = "+7" + normalized;
    } else if (normalized.length() == 11 && normalized[0] == '7') {
        normalized = '+' + normalized;
    }

    return normalized;
}

QString Contact::trim(const QString& str) const {
    return str.trimmed();
}

void Contact::spaces_email(QString& email) const {
    QString trimmed_email;
    for (const auto& element : email) {
        if (element != ' ') {
            trimmed_email.push_back(element);
        }
    }
    email = trimmed_email;
}

QString Contact::get_smallest_phone() const {
    if (m_phones.isEmpty()) return QString();

    QStringList sorted = m_phones;
    sorted.sort();
    return sorted.first();
}

QString Contact::to_string() const {
    return QString("%1 %2 %3 | Email: %4 | Phones: %5")
    .arg(m_surname, m_firstname, m_middlename, m_email, m_phones.join(", "));
}

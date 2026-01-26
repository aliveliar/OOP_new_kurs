#include <iostream>
#include <regex>
#include <ctime>
#include  "contacts.h"
using namespace std;

Contact::Contact(string firstname, string surname, string email, list <string> lst_phones, string middlename, string address, string birthday) {
    this->firstname = firstname;
    this->surname = surname;
    this->middlename = middlename;
    this->address = address;
    this->birthday = birthday;
    this->email = email;
    this->lst_phones = lst_phones;
}
Contact::Contact(const Contact &c) {
    firstname = c.firstname;
    surname = c.surname;
    middlename = c.middlename;
    address = c.address;
    birthday = c.birthday;
    email = c.email;
    lst_phones = c.lst_phones;
}

Contact::~Contact(){};

Contact& Contact::operator=(const Contact &other) {
    this->firstname = other.firstname;
    this->surname = other.surname;
    this->middlename = other.middlename;
    this->address = other.address;
    this->birthday = other.birthday;
    this->email = other.email;
    this->lst_phones = other.lst_phones;
    return *this;
}

bool Contact::operator==(const Contact &other) {
    return (firstname == other.firstname && surname == other.surname && middlename == other.middlename && address == other.address &&
        birthday == other.birthday && email == other.email && lst_phones == other.lst_phones);
}


bool Contact::valid_name(const string &name) const {
    regex pattern("^[a-zA-Zа-яА-Я][a-zA-Zа-яА-Я0-9\\-]*[a-zA-Zа-яА-Я0-9]$");
    return regex_match(trim(name), pattern);
}

bool Contact::valid_middlename(const string &name) const {
    if (name.empty()) {
        return true;
    }
    regex pattern("^[a-zA-Zа-яА-Я][a-zA-Zа-яА-Я0-9\\-]*[a-zA-Zа-яА-Я0-9]$");
    return regex_match(trim(name), pattern);
}

bool Contact::valid_birthday(const string &birthday) const {
    if (birthday.empty()) {
        return true;
    }

    if (birthday.length() != 10 || birthday[2] != '-' || birthday[5] != '-') {
        return false;
    }

    int day, month, year;
    try {
        day = stoi(birthday.substr(0, 2));
        month = stoi(birthday.substr(3, 2));
        year = stoi(birthday.substr(6, 4));
    } catch (...) {
        return false;
    }

    if (month < 1 || month > 12) {
        return false;
    }

    if (day < 1 || day > 31) {
        return false;
    }

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
        daysInMonth[1] = 29;
    }

    if (day > daysInMonth[month - 1]) {
        return false;
    }

    time_t t = time(nullptr);
    tm* now = localtime(&t);
    int currentYear = now->tm_year + 1900;
    int currentMonth = now->tm_mon + 1;
    int currentDay = now->tm_mday;


    if (year > currentYear) {
        return false;
    }
    if (year == currentYear && month > currentMonth) {
        return false;
    }
    if (year == currentYear && month == currentMonth && day > currentDay) {
        return false;
    }

    return true;
}

bool Contact::contains_firstname_in_email(const string& email, const string& firstname) const {
    if (firstname.empty() || email.empty()) return false;

    string lowercase_firstname = firstname;
    string lowercase_email = email;

    transform(lowercase_firstname.begin(), lowercase_firstname.end(), lowercase_firstname.begin(), ::tolower);
    transform(lowercase_email.begin(), lowercase_email.end(), lowercase_email.begin(), ::tolower);

    size_t at_pos = lowercase_email.find('@');
    if (at_pos == string::npos) return false;

    string local_part = lowercase_email.substr(0, at_pos);

    return local_part.find(lowercase_firstname) != string::npos;
}

bool Contact::valid_email(string &email) {
    spaces_email(email);
    regex pattern("^[a-zA-Z0-9]+@[a-zA-Z0-9]+\\.[a-zA-Z0-9]+$");
    if (!regex_match(trim(email), pattern)) {
        return false;
    }

    return contains_firstname_in_email(email, firstname);
}

bool Contact::valid_phone(const string &phone) const {
    regex pattern("^(\\+7|8)[\\s(]*(\\d{3})[\\s)]*(\\d{3})[\\s-]*(\\d{2})[\\s-]*(\\d{2})$");
    return regex_match(trim(phone), pattern);
}

bool Contact::valid_address(const string &address) const {
    return true;
}


string Contact::trim(const string &s) const{
    const string whitespaces = " \t\n";
    size_t first = s.find_first_not_of(whitespaces);
    size_t last = s.find_last_not_of(whitespaces);
    if (first == string::npos) {
        return "";
    }
    return s.substr(first, last - first + 1);
}

string Contact::get_firstname() {
    return firstname;
}

string Contact::get_surname() {
    return surname;
}

string Contact::get_middlename() {
    return middlename;
}

string Contact::get_address() {
    return address;
}

string Contact::get_birthday() {
    return  birthday;
}

string Contact::get_email() const {
    return email;
}

list<string> Contact::get_phones() {
    return lst_phones;
}

bool Contact::add_phone(const string &phone) {
    if (valid_phone(phone)) {
        this->lst_phones.push_back(normalize_phone(phone));
        return true;
    }
    return false;
}

void Contact::show() {
    cout<<"First name: " << firstname <<endl;
    cout<<"Surname: " << surname <<endl;
    cout<<"Middle name: " << ((middlename.empty()) ? "Not stated":middlename) <<endl;
    cout<<"Birthday:" << ((birthday.empty()) ? "Not stated":birthday) <<endl;
    cout<<"Address:" << ((address.empty()) ? "Not stated":address) <<endl;
    cout<<"Email: " << email <<endl;
    cout<<"Phone numbers: "<<endl;
    for (string phone: lst_phones){
        cout<<"\t"<< phone <<endl;
    }
}

bool Contact::set_firstname(string& firstname) {
    string trimmed_firstname = trim(firstname);
    if (!valid_name(trimmed_firstname)) {
        return false;
    }
    this->firstname = trimmed_firstname;
    return true;
}


bool Contact::set_surname(string& surname) {
    string trimmed_surname = trim(surname);
    if (!valid_name(trimmed_surname)) {
        return false;
    }
    this->surname = trimmed_surname;
    return true;
}

bool Contact::set_middlename(string& middlename) {
    string trimmed_middlename = trim(middlename);
    if (!valid_middlename(trimmed_middlename)) {
        return false;
    }
    this->middlename = trimmed_middlename;
    return true;
}

bool Contact::set_address(string& address) {
    this->address = trim(address);
    return true;
}

bool Contact::set_birthday(string& birthday) {
    string trimmed_birthday = trim(birthday);
    if (trimmed_birthday.empty()) {
        this->birthday = "";
        return true;
    }

    if (!valid_birthday(trimmed_birthday)) {
        return false;
    }
    this->birthday = trimmed_birthday;
    return true;
}

bool Contact::set_email(string& email) {
    string trimmed_email = trim(email);
    if (!valid_email(trimmed_email)) {
        return false;
    }
    this->email = trimmed_email;
    return true;
}

bool Contact::set_phones(list <string> lst_phones) {
    list<string> temp_lst_phones;
    for (string str_phone : lst_phones){
        string trimmed_phone = trim(str_phone);
        if (!valid_phone(trimmed_phone)) {
            return false;
        }
        temp_lst_phones.emplace_back(trimmed_phone);
    }

    this->lst_phones = temp_lst_phones;

    return true;
}

string Contact::normalize_phone(const string &phone) {
    string normal_phone;
    for (char c: phone) {
        if (isdigit(c) || c == '+') {
            normal_phone += c;
        }

    }

    if (normal_phone.length() == 11 && normal_phone[0] == '8') {
        normal_phone[0] = '7';
        normal_phone = '+' + normal_phone;
    }else if(normal_phone.length() == 10){
        normal_phone = "+7" + normal_phone;
    }else if (normal_phone.length() == 11 && normal_phone[0] == '7') {
        normal_phone = '+' + normal_phone;
    }

    return normal_phone;
}

void Contact::spaces_email(string &email) {
    string trimmed_email;
    for (const auto& element : email) {
        if (element!= ' ') {trimmed_email.push_back(element);}
    }
    email = trimmed_email;
}

int Contact::get_phones_count() const {
    return lst_phones.size();
}

bool Contact::remove_phone(int index) {
    if (index < 0 || index >= lst_phones.size()) {
        return false;
    }
    auto it = lst_phones.begin();
    advance(it, index);
    lst_phones.erase(it);
    return true;
}
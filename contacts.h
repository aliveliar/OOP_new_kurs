#pragma once
#include <string>
#include <list>
using namespace std;

class Contact{

    string firstname;
    string surname;
    string middlename;
    string address;
    string birthday;
    string email;
    list <string> lst_phones;

public:

    Contact(string firstname="", string surname="", string email="", list <string> lst_phones={}, string middlename = "", string address = "", string birthday = "");
    ~Contact();
    Contact(const Contact &c);
    Contact& operator=(const Contact& other);
    bool operator==(const Contact& other);

    bool add_phone(const string& phone);

    bool set_firstname(string& firstname);
    bool set_surname(string& surname);
    bool set_middlename(string& middlename);
    bool set_address(string& address);
    bool set_birthday(string& birthday);
    bool set_email(string& email);
    bool set_phones(list <string> lst_phone);

    string get_firstname();
    string get_surname();
    string get_middlename();
    string get_address();
    string get_birthday();
    string get_email() const;
    list <string> get_phones();
    int get_phones_count() const;


    void show();

    bool valid_middlename(const string &name) const;
    bool valid_name(const string& name) const;
    bool valid_email(string& email);
    bool valid_phone(const string& phone) const;
    bool valid_birthday(const string& birthday) const;
    bool valid_address(const string& address) const;


    string trim(const string& s) const;

    void spaces_email(string& email);

    string normalize_phone(const string& phone);
    bool remove_phone(int index);
    bool contains_firstname_in_email(const string& email, const string& firstname) const;
};
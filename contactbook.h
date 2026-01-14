#pragma once
#include "contacts.h"

class Contactbook {

    list<Contact> contactbook;
    string filename;

public:
    Contactbook(list<Contact> contactbook = {});
    Contactbook(const Contactbook& c);
    ~Contactbook();

    void show_contacts();
    void delete_contact();
    void edit_contact();
    bool add_contact(const Contact& contact);

    void sort_by_firstname();
    void sort_by_surname();
    void sort_by_middlename();
    void sort_by_address();
    void sort_by_email();
    void sort_by_birthday();
    void sort_by_phones();

    void sort_phones_in_contacts();

    bool load_from_file(const string& filename = "contacts.txt");
    bool save_to_file(const string& filename = "contacts.txt");

    Contact new_contact();

    list <Contact*> search_by_firstname(list <Contact>& contacts, const string& criteria);
    list <Contact*> search_by_surname(list <Contact>& contacts, const string& criteria);
    list <Contact*> search_by_middlename(list <Contact>& contacts, const string& criteria);
    list <Contact*> search_by_address(list <Contact>& contacts, const string& criteria);
    list <Contact*> search_by_birthday(list <Contact>& contacts, const string& criteria);
    list <Contact*> search_by_email(list <Contact>& contacts, const string& criteria);
    list <Contact*> search_by_phone(list <Contact>& contacts, const string& criteria);


    int main_menu();

    void menu_search();
    void menu_sort();
    void menu_edit(Contact& contact, int index);

    bool is_email_unique(const string& email) const;

};

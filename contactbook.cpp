#include <iostream>
#include <algorithm>
#include <limits>
#include <fstream>
#include "contactbook.h"

using namespace std;

Contactbook::Contactbook(list<Contact> contactbook) {
    this->contactbook = contactbook;
}

Contactbook::Contactbook(const Contactbook& c){
    contactbook = c.contactbook;
}

Contactbook::~Contactbook(){};

bool Contactbook::add_contact(const Contact& contact){
    if (!is_email_unique(contact.get_email())) {
        cout << "Error: Email already exists in the phone book." << endl;
        return false;
    }

    contactbook.push_back(contact);
    cout << "Contact successfully added!" << endl;
    return true;
}

void Contactbook::delete_contact() {
    if (contactbook.empty()) {
        cout << "Phone book is empty." << endl;
        return;
    }

    show_contacts();

    int index;
    cout << "Enter contact number to delete: "<<endl;
    cin >> index;
    index--;

    if (index < 0 || contactbook.size() <= index) {
            cout << "Index out of phone book bounds." << endl;
    }
    else {
        auto iter = contactbook.begin();
        advance(iter, index);
        contactbook.erase(iter);
        index++;
        cout << "Contact number " << index << " successfully deleted."  << endl;
    }
}

int Contactbook::main_menu() {
    int opt;
    bool continue_ = true;

    load_from_file();

    while (continue_) {
        cout << "\n=== PHONE BOOK ===" << endl;
        cout << "1. View phone book" << endl;
        cout << "2. Create contact" << endl;
        cout << "3. Edit contact" << endl;
        cout << "4. Delete contact" << endl;
        cout << "5. Search contacts" << endl;
        cout << "6. Sort contacts" << endl;
        cout << "7. Save to file" << endl;
        cout << "8. Exit program" << endl;
        cout << "Choose option: "<<endl;
        cin >> opt;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opt) {
            case 1:
                show_contacts();
                break;
            case 2:
            {
                Contact new_contact = this->new_contact();
                add_contact(new_contact);
            }
                break;
            case 3:
                edit_contact();
                break;
            case 4:
                delete_contact();
                break;
            case 5:
                menu_search();
                break;
            case 6:
                menu_sort();
                break;
            case 7:
                save_to_file();
                break;
            case 8:
                cout << "Exiting program..." << endl;
                save_to_file();
                continue_ = false;
                break;
            default:
                cout << "Invalid option! Please try again." << endl;
        }
    }
}

Contact Contactbook::new_contact() {
    Contact contact;
    string input;

    cout << "=== CREATE NEW CONTACT ===" << endl;

    do {
        cout << "Enter first name (required):"<<endl;;
        getline(cin, input);
    } while (input.empty() || !contact.set_firstname(input));

    do {
        cout << "Enter surname (required):"<<endl;;
        getline(cin, input);
    } while (input.empty() || !contact.set_surname(input));
    cout<<endl;

    cout << "Enter middle name (optional, press Enter to skip):"<<endl;
    getline(cin, input);
    if (!input.empty()) {
        while (!contact.set_middlename(input)) {
            cout << "Error: Invalid middle name format. Please enter a valid middle name or press Enter to skip:" << endl;
            getline(cin, input);
            if (input.empty()) {
                break;
            }
        }
    }

    do {
        cout << "Enter email (required):"<<endl;
        getline(cin, input);
    } while (input.empty() || !contact.set_email(input));

    do {
        cout << "Enter phone number (required):"<<endl;
        getline(cin, input);
    } while (input.empty() || !contact.add_phone(input));
    cout << "Phone number added successfully!" << endl;

    char choice;
    do {
        cout << "Add another phone number? (y/n):"<<endl;
        cin >> choice;
        cin.ignore();

        if (choice == 'y' || choice == 'Y') {
            cout << "Enter phone number:"<<endl;
            getline(cin, input);
            if (!input.empty()) {
                if (contact.add_phone(input)) {
                    cout << "Phone number added successfully!" << endl;
                } else {
                    cout << "Invalid phone number format!" << endl;
                }
            }
        }
    } while (choice == 'y' || choice == 'Y');

    cout << "Enter birthday (DD-MM-YYYY, optional):"<<endl;
    getline(cin, input);
    if (!input.empty()) {
        while (!contact.set_birthday(input)) {
            cout << "Error: Invalid birthday format or date is in the future. Please enter a valid birthday (DD-MM-YYYY) or press Enter to skip:" << endl;
            getline(cin, input);
            if (input.empty()) {
                break;
            }
        }
    }

    cout << "Enter address (optional):"<<endl;
    getline(cin, input);
    if (!input.empty()) {
        while (!contact.set_address(input)) {
            cout << "Error: Invalid address. Please enter a valid address or press Enter to skip:" << endl;
            getline(cin, input);
            if (input.empty()) {
                break;
            }
        }
    }

    cout << "\n=== NEW CONTACT CREATED ===" << endl;
    contact.show();

    return contact;
}


void Contactbook::show_contacts() {
    if (contactbook.empty()) {
        cout << "Phone book is empty." << endl;
        return;
    }

    cout<<"\n-------LIST OF CONTACTS-------\n"<<endl;
    int index = 1;
    for (Contact& contact : contactbook){
        cout << "=== Contact #" << index++ << " ===" << endl;
        contact.show();
        cout<<endl;
    }
}

void Contactbook::edit_contact() {
    if (contactbook.empty()) {
        cout << "Phone book is empty." << endl;
        return;
    }

    show_contacts();
    int index;
    cout << "Enter the contact number you want to edit: ";
    cin >> index;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    index--;

    if (index < 0 || contactbook.size() <= index) {
        cout << "Index out of phone book bounds." << endl;
        return;
    }

    auto iter = contactbook.begin();
    advance(iter, index);
    Contact& contact = *iter;

    menu_edit(contact, index + 1);
}


void Contactbook::menu_edit(Contact& contact, int index) {
    int choice;
    bool continue_editing = true;
    while (continue_editing) {
        cout << "\n=== Edit Contact #" << index << " ===" << endl;
        cout << "Current contact data:" << endl;
        contact.show();

        cout << "\nChoose field to edit:" << endl;
        cout << "1. First name" << endl;
        cout << "2. Surname" << endl;
        cout << "3. Middle name" << endl;
        cout << "4. Address" << endl;
        cout << "5. Birthday" << endl;
        cout << "6. Email" << endl;
        cout << "7. Phone numbers" << endl;
        cout << "8. Finish editing" << endl;
        cout << "Choose option: ";

        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string input;
        bool success = false;

        switch (choice) {
            case 1:
                cout << "Current first name: " << contact.get_firstname() << endl;
                cout << "Enter new first name: ";
                getline(cin, input);
                success = contact.set_firstname(input);
                if (success) {
                    cout << "First name updated successfully!" << endl;

                    string new_email;
                    bool email_valid = false;

                    while (!email_valid) {
                        cout << "You must update your email to include your new first name." << endl;
                        cout << "Enter new email: ";
                        getline(cin, new_email);

                        if (!is_email_unique(new_email)) {
                            cout << "Error: This email already exists in the phone book." << endl;
                            continue;
                        }

                        if (contact.set_email(new_email)) {
                            email_valid = true;
                            cout << "Email updated successfully!" << endl;
                        } else {
                            cout << "Error: Invalid email format or email doesn't contain your first name." << endl;
                        }
                    }
                } else {
                    cout << "Error: Invalid first name format." << endl;
                }
                break;

            case 2:
                cout << "Current surname: " << contact.get_surname() << endl;
                cout << "Enter new surname: ";
                getline(cin, input);
                success = contact.set_surname(input);
                if (success) {
                    cout << "Surname updated successfully!" << endl;
                } else {
                    cout << "Error: Invalid surname format." << endl;
                }
                break;

            case 3:
                cout << "Current middle name: " <<
                     (contact.get_middlename().empty() ? "Not set" : contact.get_middlename()) << endl;
                cout << "Enter new middle name (or press Enter to clear): ";
                getline(cin, input);
                success = contact.set_middlename(input);
                if (success) {
                    cout << "Middle name updated successfully!" << endl;
                } else {
                    cout << "Error: Invalid middle name format." << endl;
                }
                break;

            case 4:
                cout << "Current address: " <<
                     (contact.get_address().empty() ? "Not set" : contact.get_address()) << endl;
                cout << "Enter new address: ";
                getline(cin, input);
                success = contact.set_address(input);
                if (success) {
                    cout << "Address updated successfully!" << endl;
                } else {
                    cout << "Error: Invalid address." << endl;
                }
                break;

            case 5:
                cout << "Current your birthday: " <<
                     (contact.get_birthday().empty() ? "Not set" : contact.get_birthday()) << endl;
                cout << "Enter new birthday (DD-MM-YYYY): ";
                getline(cin, input);
                success = contact.set_birthday(input);
                if (success) {
                    cout << "Birthday updated successfully!" << endl;
                } else {
                    cout << "Error: Invalid birthday format or date is in the future." << endl;
                }
                break;

            case 6:
                cout << "Current email: " << contact.get_email() << endl;
                cout << "Enter new email: ";
                getline(cin, input);

                if (input != contact.get_email() && !is_email_unique(input)) {
                    cout << "Error: This email already exists in the phone book." << endl;
                } else {
                    success = contact.set_email(input);
                    if (success) {
                        cout << "Email updated successfully!" << endl;
                    } else {
                        cout << "Error: Invalid email format or email doesn't contain your first name." << endl;
                    }
                }
                break;

            case 7:
            {
                int phoneChoice;
                bool continuePhoneEditing = true;

                while (continuePhoneEditing) {
                    cout << "\n--- Phone Numbers Management ---" << endl;
                    cout << "Current phone numbers:" << endl;
                    list<string> phones = contact.get_phones();
                    int phoneIndex = 1;
                    for (const auto& phone : phones) {
                        cout << phoneIndex << ". " << phone << endl;
                        phoneIndex++;
                    }

                    cout << "\nChoose action:" << endl;
                    cout << "1. Add new phone number" << endl;
                    cout << "2. Remove phone number" << endl;
                    cout << "3. Back to main menu" << endl;
                    cout << "Choose option: ";

                    cin >> phoneChoice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    switch (phoneChoice) {
                        case 1:
                        {
                            string newPhone;
                            cout << "Enter new phone number: ";
                            getline(cin, newPhone);
                            bool phoneSuccess = contact.add_phone(newPhone);
                            if (phoneSuccess) {
                                cout << "Phone number added successfully!" << endl;
                            } else {
                                cout << "Error: Invalid phone number format." << endl;
                            }
                        }
                            break;

                        case 2:
                            if (phones.empty()) {
                                cout << "No phone numbers to remove." << endl;
                            } else {
                                int removeIndex;
                                cout << "Enter phone number to remove (1-" << phones.size() << "): ";
                                cin >> removeIndex;
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                                if (removeIndex < 1 || removeIndex > phones.size()) {
                                    cout << "Invalid phone number index." << endl;
                                } else {
                                    if (contact.remove_phone(removeIndex - 1)) {
                                        cout << "Phone number removed successfully!" << endl;
                                    } else {
                                        cout << "Error: Failed to remove phone number." << endl;
                                    }
                                }
                            }
                            break;

                        case 3:
                            continuePhoneEditing = false;
                            break;

                        default:
                            cout << "Invalid option. Please try again." << endl;
                            break;
                    }

                    if (phoneChoice != 3) {
                        cout << "\nPress Enter to continue...";
                        cin.get();
                    }
                }
            }
                break;

            case 8:
                continue_editing = false;
                cout << "Editing completed. Contact #" << index << " updated successfully!" << endl;
                break;

            default:
                cout << "Invalid option. Please try again." << endl;
                break;
        }
    }
}

void Contactbook::sort_phones_in_contacts() {
    for (Contact& contact : contactbook) {
        list<string> phones = contact.get_phones();
        phones.sort();
        contact.set_phones(phones);
    }
}

void Contactbook::menu_search() {
    if (contactbook.empty()) {
        cout << "Phone book is empty." << endl;
        return;
    }

    list<Contact*> results;
    for (Contact& contact : contactbook) {
        results.push_back(&contact);
    }

    bool continue_searching = true;

    while (continue_searching && !results.empty()) {
        cout << "\n=== CONTACTS SEARCH ===" << endl;
        cout << "Current results: " << results.size() << " contacts" << endl;

        int choice;
        cout << "1. Search by first name" << endl;
        cout << "2. Search by surname" << endl;
        cout << "3. Search by middle name" << endl;
        cout << "4. Search by email" << endl;
        cout << "5. Search by phone" << endl;
        cout << "6. Search by address" << endl;
        cout << "7. Search by birthday" << endl;
        cout << "8. Show current results" << endl;
        cout << "9. Finish search" << endl;
        cout << "Choose option: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 9) {
            continue_searching = false;
            break;
        }

        if (choice == 8) {
            cout << "\n=== CURRENT SEARCH RESULTS ===" << endl;
            int index = 1;
            for (Contact* contact : results) {
                cout << "=== Contact #" << index++ << " ===" << endl;
                contact->show();
                cout << endl;
            }
            continue;
        }

        string criteria;
        cout << "Enter search criteria: ";
        getline(cin, criteria);

        list<Contact> temp_contacts;
        for (Contact* contact_ptr : results) {
            temp_contacts.push_back(*contact_ptr);
        }

        list<Contact*> new_results;

        switch (choice) {
            case 1:
                new_results = search_by_firstname(temp_contacts, criteria);
                break;
            case 2:
                new_results = search_by_surname(temp_contacts, criteria);
                break;
            case 3:
                new_results = search_by_middlename(temp_contacts, criteria);
                break;
            case 4:
                new_results = search_by_email(temp_contacts, criteria);
                break;
            case 5:
                new_results = search_by_phone(temp_contacts, criteria);
                break;
            case 6:
                new_results = search_by_address(temp_contacts, criteria);
                break;
            case 7:
                new_results = search_by_birthday(temp_contacts, criteria);
                break;
            default:
                cout << "Invalid option!" << endl;
                continue;
        }

        if (new_results.empty()) {
            cout << "No contacts found with the specified criteria." << endl;
            cout << "Do you want to continue with current results? (y/n): ";
            char answer;
            cin >> answer;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (answer == 'n' || answer == 'N') {
                continue_searching = false;
            }
        } else {
            results = new_results;
            cout << "Found " << results.size() << " contacts." << endl;

            if (results.size() > 0) {
                cout << "Do you want to continue searching? (y/n): ";
                char answer;
                cin >> answer;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (answer == 'n' || answer == 'N') {
                    continue_searching = false;
                }
            }
        }
    }

    if (!results.empty()) {
        cout << "\n=== FINAL SEARCH RESULTS ===" << endl;
        int index = 1;
        for (Contact* contact : results) {
            cout << "=== Contact #" << index++ << " ===" << endl;
            contact->show();
            cout << endl;
        }
    } else {
        cout << "No contacts found." << endl;
    }
}

void Contactbook::menu_sort() {
    if (contactbook.empty()) {
        cout << "Phone book is empty." << endl;
        return;
    }

    int choice;
    cout << "\n=== CONTACTS SORTING ===" << endl;
    cout << "1. Sort by first name" << endl;
    cout << "2. Sort by surname" << endl;
    cout << "3. Sort by middle name" << endl;
    cout << "4. Sort by email" << endl;
    cout << "5. Sort by birthday" << endl;
    cout << "6. Sort by address" << endl;
    cout << "7. Sort by phone numbers" << endl;
    cout << "Choose option: ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (choice) {
        case 1:
            sort_by_firstname();
            break;
        case 2:
            sort_by_surname();
            break;
        case 3:
            sort_by_middlename();
            break;
        case 4:
            sort_by_email();
            break;
        case 5:
            sort_by_birthday();
            break;
        case 6:
            sort_by_address();
            break;
        case 7:
            sort_by_phones();
            break;
        default:
            cout << "Invalid option!" << endl;
            return;
    }

    cout << "Contacts successfully sorted!" << endl;
}

void Contactbook::sort_by_firstname() {
    contactbook.sort([](Contact& a, Contact& b) {
        return a.get_firstname() < b.get_firstname();
    });
}

void Contactbook::sort_by_surname() {
    contactbook.sort([](Contact& a, Contact& b) {
        return a.get_surname() < b.get_surname();
    });
}

void Contactbook::sort_by_middlename() {
    contactbook.sort([](Contact& a, Contact& b) {
        return a.get_middlename() < b.get_middlename();
    });
}

void Contactbook::sort_by_address() {
    contactbook.sort([](Contact& a, Contact& b) {
        return a.get_address() < b.get_address();
    });
}

void Contactbook::sort_by_email() {
    contactbook.sort([](Contact& a, Contact& b) {
        return a.get_email() < b.get_email();
    });
}

void Contactbook::sort_by_birthday() {
    contactbook.sort([](Contact& a, Contact& b) {
        return a.get_birthday() < b.get_birthday();
    });
}

void Contactbook::sort_by_phones() {
    sort_phones_in_contacts();

    contactbook.sort([](Contact& a, Contact& b) {
        list<string> phones_a = a.get_phones();
        list<string> phones_b = b.get_phones();

        return phones_a.front() < phones_b.front();
    });
}

list<Contact*> Contactbook::search_by_firstname(list<Contact>& contacts, const string& criteria) {
    list<Contact*> results;
    for (Contact& contact : contacts) {
        if (contact.get_firstname().find(criteria) != string::npos) {
            results.push_back(&contact);
        }
    }
    return results;
}

list<Contact*> Contactbook::search_by_surname(list<Contact>& contacts, const string& criteria) {
    list<Contact*> results;
    for (Contact& contact : contacts) {
        if (contact.get_surname().find(criteria) != string::npos) {
            results.push_back(&contact);
        }
    }
    return results;
}

list<Contact*> Contactbook::search_by_middlename(list<Contact>& contacts, const string& criteria) {
    list<Contact*> results;
    for (Contact& contact : contacts) {
        if (contact.get_middlename().find(criteria) != string::npos) {
            results.push_back(&contact);
        }
    }
    return results;
}

list<Contact*> Contactbook::search_by_address(list<Contact>& contacts, const string& criteria) {
    list<Contact*> results;
    for (Contact& contact : contacts) {
        if (contact.get_address().find(criteria) != string::npos) {
            results.push_back(&contact);
        }
    }
    return results;
}

list<Contact*> Contactbook::search_by_birthday(list<Contact>& contacts, const string& criteria) {
    list<Contact*> results;
    for (Contact& contact : contacts) {
        if (contact.get_birthday().find(criteria) != string::npos) {
            results.push_back(&contact);
        }
    }
    return results;
}

list<Contact*> Contactbook::search_by_email(list<Contact>& contacts, const string& criteria) {
    list<Contact*> results;
    for (Contact& contact : contacts) {
        if (contact.get_email().find(criteria) != string::npos) {
            results.push_back(&contact);
        }
    }
    return results;
}

list<Contact*> Contactbook::search_by_phone(list<Contact>& contacts, const string& criteria) {
    list<Contact*> results;
    for (Contact& contact : contacts) {
        for (const string& phone : contact.get_phones()) {
            if (phone.find(criteria) != string::npos) {
                results.push_back(&contact);
                break;
            }
        }
    }
    return results;
}

bool Contactbook::load_from_file(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "File not found. A new file will be created when saving." << endl;
        return false;
    }

    contactbook.clear();
    string line;
    int contacts_loaded = 0;

    while (getline(file, line)) {
        if (line == "===CONTACT===") {
            string firstname, surname, middlename, address, birthday, email;
            list<string> phones;

            if (!getline(file, firstname)) break;
            if (!getline(file, surname)) break;
            if (!getline(file, middlename)) break;
            if (!getline(file, address)) break;
            if (!getline(file, birthday)) break;
            if (!getline(file, email)) break;

            string phone;
            while (getline(file, phone)) {
                if (phone == "===END===") {
                    break;
                }
                if (!phone.empty()) {
                    phones.push_back(phone);
                }
            }

            Contact contact(firstname, surname, email, phones, middlename, address, birthday);
            contactbook.push_back(contact);
            contacts_loaded++;
        }
    }

    file.close();
    cout << "Successfully loaded " << contacts_loaded << " contacts from file." << endl;
    return contacts_loaded > 0;
}

bool Contactbook::save_to_file(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << " for writing." << endl;
        return false;
    }

    int contacts_saved = 0;
    for (Contact& contact : contactbook) {
        file << "===CONTACT===\n";
        file << contact.get_firstname() << "\n";
        file << contact.get_surname() << "\n";
        file << contact.get_middlename() << "\n";
        file << contact.get_address() << "\n";
        file << contact.get_birthday() << "\n";
        file << contact.get_email() << "\n";

        for (const string& phone : contact.get_phones()) {
            file << phone << "\n";
        }

        file << "===END===\n";
        contacts_saved++;
    }

    file.close();

    if (file.fail()) {
        cout << "Error: Failed to write to file." << endl;
        return false;
    }

    cout << "Successfully saved " << contacts_saved << " contacts to file." << endl;
    return true;
}

bool Contactbook::is_email_unique(const string& email) const {
    for (const Contact& contact : contactbook) {
        if (contact.get_email() == email) {
            return false;
        }
    }
    return true;
}
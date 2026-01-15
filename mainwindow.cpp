#include "mainwindow.h"
#include "contactdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QMenuBar>
#include <QFileDialog>
#include <QApplication>
#include <QInputDialog>
#include <QGroupBox>
#include <QFormLayout>
#include <QScrollArea>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent),
    m_contactbook(new Contactbook(this)),
    m_database(new Database(this))
{
    setup_UI();
    create_menu();
    refresh_table();
    connect(m_contactbook, &Contactbook::contacts_changed, this, &MainWindow::refresh_table);
}

MainWindow::~MainWindow() {}

// Настройка пользовательского интерфейса главного окна
void MainWindow::setup_UI() {
    setWindowTitle("Phone Book");
    setMinimumSize(1000, 700);

    m_table = new QTableWidget(this);
    m_table->setColumnCount(7);
    m_table->setHorizontalHeaderLabels({"First Name", "Surname", "Middle Name", "Email", "Phones", "Birthday", "Address"});
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QGroupBox* searchGroup = new QGroupBox("Search Contacts");
    QGridLayout* searchLayout = new QGridLayout;

    m_search_firstname = new QLineEdit;
    m_search_surname = new QLineEdit;
    m_search_middlename = new QLineEdit;
    m_search_email = new QLineEdit;
    m_search_phone = new QLineEdit;
    m_search_address = new QLineEdit;
    m_search_birthday = new QLineEdit;

    searchLayout->addWidget(new QLabel("First Name:"), 0, 0);
    searchLayout->addWidget(m_search_firstname, 0, 1);
    searchLayout->addWidget(new QLabel("Surname:"), 0, 2);
    searchLayout->addWidget(m_search_surname, 0, 3);

    searchLayout->addWidget(new QLabel("Middle Name:"), 1, 0);
    searchLayout->addWidget(m_search_middlename, 1, 1);
    searchLayout->addWidget(new QLabel("Email:"), 1, 2);
    searchLayout->addWidget(m_search_email, 1, 3);

    searchLayout->addWidget(new QLabel("Phone:"), 2, 0);
    searchLayout->addWidget(m_search_phone, 2, 1);
    searchLayout->addWidget(new QLabel("Birthday:"), 2, 2);
    searchLayout->addWidget(m_search_birthday, 2, 3);

    searchLayout->addWidget(new QLabel("Address:"), 3, 0);
    searchLayout->addWidget(m_search_address, 3, 1);

    QHBoxLayout* buttonSearchLayout = new QHBoxLayout;
    m_search_button = new QPushButton("Search");
    m_clear_search_button = new QPushButton("Clear Search");
    buttonSearchLayout->addWidget(m_search_button);
    buttonSearchLayout->addWidget(m_clear_search_button);
    buttonSearchLayout->addStretch();

    searchLayout->addLayout(buttonSearchLayout, 4, 0, 1, 4);
    searchGroup->setLayout(searchLayout);

    QWidget* buttonPanel = new QWidget;
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    m_add_button = new QPushButton("Add Contact");
    m_edit_button = new QPushButton("Edit Contact");
    m_delete_button = new QPushButton("Delete Contact");
    m_sort_button = new QPushButton("Sort");
    m_save_button = new QPushButton("Save");
    m_load_button = new QPushButton("Load");
    buttonLayout->addWidget(m_add_button);
    buttonLayout->addWidget(m_edit_button);
    buttonLayout->addWidget(m_delete_button);
    buttonLayout->addWidget(m_sort_button);
    buttonLayout->addWidget(m_save_button);
    buttonLayout->addWidget(m_load_button);
    buttonPanel->setLayout(buttonLayout);

    QWidget* central = new QWidget;
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(searchGroup);
    mainLayout->addWidget(m_table, 1);
    mainLayout->addWidget(buttonPanel);
    central->setLayout(mainLayout);
    setCentralWidget(central);

    QPushButton* saveDbButton = new QPushButton("Save to DB");
    QPushButton* loadDbButton = new QPushButton("Load from DB");
    buttonLayout->addWidget(saveDbButton);
    buttonLayout->addWidget(loadDbButton);

    connect(m_add_button, &QPushButton::clicked, this, &MainWindow::on_add_contact);
    connect(m_edit_button, &QPushButton::clicked, this, &MainWindow::on_edit_contact);
    connect(m_delete_button, &QPushButton::clicked, this, &MainWindow::on_delete_contact);
    connect(m_search_button, &QPushButton::clicked, this, &MainWindow::on_search);
    connect(m_clear_search_button, &QPushButton::clicked, this, &MainWindow::on_clear_search);
    connect(m_sort_button, &QPushButton::clicked, this, &MainWindow::on_sort);
    connect(m_save_button, &QPushButton::clicked, this, &MainWindow::on_save);
    connect(m_load_button, &QPushButton::clicked, this, &MainWindow::on_load);
    connect(saveDbButton, &QPushButton::clicked, this, &MainWindow::on_save_to_database);
    connect(loadDbButton, &QPushButton::clicked, this, &MainWindow::on_load_from_database);
}

// Создание меню приложения
void MainWindow::create_menu() {
    QMenu* fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("Load", this, &MainWindow::on_load);
    fileMenu->addAction("Save", this, &MainWindow::on_save);
    fileMenu->addSeparator();
    fileMenu->addAction("Exit", qApp, &QApplication::quit);

    QMenu* editMenu = menuBar()->addMenu("Edit");
    editMenu->addAction("Add Contact", this, &MainWindow::on_add_contact);
    editMenu->addAction("Edit Contact", this, &MainWindow::on_edit_contact);
    editMenu->addAction("Delete Contact", this, &MainWindow::on_delete_contact);

    QMenu* viewMenu = menuBar()->addMenu("View");
    viewMenu->addAction("Sort", this, &MainWindow::on_sort);
    viewMenu->addAction("Refresh", this, &MainWindow::refresh_table);
}

// Обновление таблицы контактов
void MainWindow::refresh_table() {
    m_table->setRowCount(0);
    QList<Contact> contacts = m_contactbook->get_all_contacts();
    for (int i = 0; i < contacts.size(); ++i) {
        const Contact& c = contacts[i];
        m_table->insertRow(i);
        m_table->setItem(i, 0, new QTableWidgetItem(c.get_firstname()));
        m_table->setItem(i, 1, new QTableWidgetItem(c.get_surname()));
        m_table->setItem(i, 2, new QTableWidgetItem(c.get_middlename()));
        m_table->setItem(i, 3, new QTableWidgetItem(c.get_email()));
        m_table->setItem(i, 4, new QTableWidgetItem(c.get_phones().join(", ")));
        m_table->setItem(i, 5, new QTableWidgetItem(c.get_birthday()));
        m_table->setItem(i, 6, new QTableWidgetItem(c.get_address()));
    }
}

// Добавление нового контакта через диалоговое окно
void MainWindow::on_add_contact() {
    ContactDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Contact contact = dialog.get_contact();

        QString errorMsg;
        if (contact.get_firstname().isEmpty()) {
            errorMsg = "First name is required!";
        } else if (contact.get_surname().isEmpty()) {
            errorMsg = "Surname is required!";
        } else if (contact.get_email().isEmpty()) {
            errorMsg = "Email is required!";
        } else if (contact.get_phones().isEmpty()) {
            errorMsg = "At least one phone number is required!";
        }

        if (!errorMsg.isEmpty()) {
            QMessageBox::warning(this, "Error", errorMsg);
            return;
        }

        if (!contact.valid_email(contact.get_email())) {
            QMessageBox::warning(this, "Error", "Email must contain your first name!");
            return;
        }

        if (!m_contactbook->is_email_unique(contact.get_email())) {
            QMessageBox::warning(this, "Error", "This email already exists in the phone book!");
            return;
        }

        if (m_contactbook->add_contact(contact)) {
            QMessageBox::information(this, "Success", "Contact added successfully!");
        } else {
            show_contact_error(contact, false);
        }
    }
}

// Редактирование существующего контакта через диалоговое окно
void MainWindow::on_edit_contact() {
    int row = m_table->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Warning", "Please select a contact to edit.");
        return;
    }

    Contact oldContact = m_contactbook->get_contact(row);
    ContactDialog dialog(this);
    dialog.set_contact(oldContact);
    if (dialog.exec() == QDialog::Accepted) {
        Contact newContact = dialog.get_contact();

        QString errorMsg = newContact.validate_for_edit();
        if (!errorMsg.isEmpty()) {
            QMessageBox::warning(this, "Validation Error", errorMsg);
            return;
        }

        if (!newContact.valid_email(newContact.get_email())) {
            QMessageBox::warning(this, "Error", "Email must contain your first name!");
            return;
        }

        if (!m_contactbook->update_contact(row, newContact)) {
            if (!m_contactbook->is_email_unique(newContact.get_email(), row)) {
                QMessageBox::warning(this, "Error", "This email already exists in the phone book!");
            } else {
                show_contact_error(newContact, true);
            }
        } else {
            QMessageBox::information(this, "Success", "Contact updated successfully!");
        }
    }
}

void MainWindow::show_contact_error(const Contact& contact, bool isEditing) {
    QString errorMsg = "Invalid contact data:\n";

    if (contact.get_firstname().isEmpty()) {
        errorMsg += "- First name is required\n";
    } else if (!contact.valid_name(contact.get_firstname())) {
        errorMsg += "- Invalid first name format\n";
    }

    if (contact.get_surname().isEmpty()) {
        errorMsg += "- Surname is required\n";
    } else if (!contact.valid_name(contact.get_surname())) {
        errorMsg += "- Invalid surname format\n";
    }

    if (!contact.get_middlename().isEmpty() && !contact.valid_name(contact.get_middlename())) {
        errorMsg += "- Invalid middle name format\n";
    }

    if (contact.get_email().isEmpty()) {
        errorMsg += "- Email is required\n";
    } else if (!contact.valid_email(contact.get_email())) {
        errorMsg += "- Invalid email format or email doesn't contain your first name\n";
    }

    if (contact.get_phones().isEmpty()) {
        errorMsg += "- At least one phone number is required\n";
    } else {
        for (const QString& phone : contact.get_phones()) {
            if (!contact.valid_phone(phone)) {
                errorMsg += "- Invalid phone number format\n";
                break;
            }
        }
    }

    if (isEditing && !contact.get_birthday().isEmpty() && !contact.valid_birthday(contact.get_birthday(), false)) {
        errorMsg += "- Invalid birthday format or date is in the future\n";
    }

    QMessageBox::warning(this, "Error", errorMsg);
}

// Удаление выбранного контакта
void MainWindow::on_delete_contact() {
    int row = m_table->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Warning", "Please select a contact to delete.");
        return;
    }

    if (QMessageBox::question(this, "Confirm", "Are you sure you want to delete this contact?") == QMessageBox::Yes) {
        m_contactbook->remove_contact(row);
        QMessageBox::information(this, "Success", "Contact deleted successfully!");
    }
}

// Поиск контактов по различным критериям
void MainWindow::on_search() {
    QList<Contact> results = m_contactbook->get_all_contacts();

    if (!m_search_firstname->text().isEmpty()) {
        results = m_contactbook->search_by_firstname(results, m_search_firstname->text());
    }
    if (!m_search_surname->text().isEmpty()) {
        results = m_contactbook->search_by_surname(results, m_search_surname->text());
    }
    if (!m_search_middlename->text().isEmpty()) {
        results = m_contactbook->search_by_middlename(results, m_search_middlename->text());
    }
    if (!m_search_email->text().isEmpty()) {
        results = m_contactbook->search_by_email(results, m_search_email->text());
    }
    if (!m_search_phone->text().isEmpty()) {
        results = m_contactbook->search_by_phone(results, m_search_phone->text());
    }
    if (!m_search_address->text().isEmpty()) {
        results = m_contactbook->search_by_address(results, m_search_address->text());
    }
    if (!m_search_birthday->text().isEmpty()) {
        results = m_contactbook->search_by_birthday(results, m_search_birthday->text());
    }

    m_table->setRowCount(0);
    for (int i = 0; i < results.size(); ++i) {
        const Contact& c = results[i];
        m_table->insertRow(i);
        m_table->setItem(i, 0, new QTableWidgetItem(c.get_firstname()));
        m_table->setItem(i, 1, new QTableWidgetItem(c.get_surname()));
        m_table->setItem(i, 2, new QTableWidgetItem(c.get_middlename()));
        m_table->setItem(i, 3, new QTableWidgetItem(c.get_email()));
        m_table->setItem(i, 4, new QTableWidgetItem(c.get_phones().join(", ")));
        m_table->setItem(i, 5, new QTableWidgetItem(c.get_birthday()));
        m_table->setItem(i, 6, new QTableWidgetItem(c.get_address()));
    }

    if (results.isEmpty()) {
        QMessageBox::information(this, "Search", "No contacts found with specified criteria.");
    }
}

// Очистка полей поиска и восстановление полного списка контактов
void MainWindow::on_clear_search() {
    m_search_firstname->clear();
    m_search_surname->clear();
    m_search_middlename->clear();
    m_search_email->clear();
    m_search_phone->clear();
    m_search_address->clear();
    m_search_birthday->clear();

    refresh_table();
}

// Сортировка контактов по выбранному критерию через диалоговое окно
void MainWindow::on_sort() {
    QStringList options = {"First Name", "Surname", "Middle Name", "Email", "Birthday", "Address", "Phones"};
    bool ok;
    QString choice = QInputDialog::getItem(this, "Sort", "Sort by:", options, 0, false, &ok);
    if (!ok) return;

    if (choice == "First Name") m_contactbook->sort_by_firstname();
    else if (choice == "Surname") m_contactbook->sort_by_surname();
    else if (choice == "Middle Name") m_contactbook->sort_by_middlename();
    else if (choice == "Email") m_contactbook->sort_by_email();
    else if (choice == "Birthday") m_contactbook->sort_by_birthday();
    else if (choice == "Address") m_contactbook->sort_by_address();
    else if (choice == "Phones") m_contactbook->sort_by_phones();
}

// Сохранение контактов в файл через диалоговое окно выбора файла
void MainWindow::on_save() {
    QString filename = QFileDialog::getSaveFileName(this, "Save Contacts", "", "Text Files (*.txt);;All Files (*)");
    if (!filename.isEmpty()) {
        if (m_contactbook->save_to_file(filename)) {
            QMessageBox::information(this, "Success", "Contacts saved successfully!");
        } else {
            QMessageBox::warning(this, "Error", "Failed to save contacts!");
        }
    } else {
        if (m_contactbook->save_to_file("contacts.txt")) {
            QMessageBox::information(this, "Success", "Contacts saved to default file (contacts.txt)!");
        } else {
            QMessageBox::warning(this, "Error", "Failed to save contacts!");
        }
    }
}

// Загрузка контактов из файла через диалоговое окно выбора файла
void MainWindow::on_load() {
    QString filename = QFileDialog::getOpenFileName(this, "Load Contacts", "", "Text Files (*.txt);;All Files (*)");
    if (!filename.isEmpty()) {
        if (m_contactbook->load_from_file(filename)) {
            QMessageBox::information(this, "Success", "Contacts loaded successfully!");
        } else {
            QMessageBox::warning(this, "Error", "Failed to load contacts!");
        }
    } else {
        if (m_contactbook->load_from_file("contacts.txt")) {
            QMessageBox::information(this, "Success", "Contacts loaded from default file (contacts.txt)!");
        } else {
            QMessageBox::warning(this, "Error", "Failed to load contacts!");
        }
    }
}

// Сохранение контактов в базу данных PostgreSQL
void MainWindow::on_save_to_database()
{
    if (!m_database->connect_to_database()) {
        QMessageBox::warning(this, "Error",
                             "Failed to connect to PostgreSQL. Check:\n"
                             "1. Is PostgreSQL server running\n"
                             "2. Connection settings\n"
                             "3. Does database 'pgs' exist");
        return;
    }

    QList<Contact> contacts = m_contactbook->get_all_contacts();
    if (m_database->save_contacts(contacts)) {
        QMessageBox::information(this, "Success",
                                 QString("Saved %1 contacts to database").arg(contacts.size()));
    } else {
        QMessageBox::warning(this, "Error", "Failed to save contacts to database");
    }
}

// Загрузка контактов из базы данных PostgreSQL
void MainWindow::on_load_from_database()
{
    if (!m_database->connect_to_database()) {
        QMessageBox::warning(this, "Error", "Failed to connect to PostgreSQL");
        return;
    }

    QList<Contact> contacts = m_database->load_contacts();

    if (!contacts.isEmpty()) {
        while (m_contactbook->contact_count() > 0) {
            m_contactbook->remove_contact(0);
        }

        int addedCount = 0;
        for (const Contact& contact : contacts) {
            if (m_contactbook->add_contact(contact)) {
                addedCount++;
            }
        }

        refresh_table();

        QMessageBox::information(this, "Success",
                                 QString("Loaded %1 contacts from database").arg(addedCount));

        qDebug() << "Contacts loaded:" << addedCount;
        qDebug() << "Now in Contactbook:" << m_contactbook->contact_count() << "contacts";

    } else {
        QMessageBox::information(this, "Information",
                                 "No contacts in database or table is empty");
    }
}

#include "contactdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>

ContactDialog::ContactDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Contact");
    setMinimumWidth(500);

    QVBoxLayout* mainLayout = new QVBoxLayout;

    QFormLayout* formLayout = new QFormLayout;

    m_firstname_edit = new QLineEdit;
    m_surname_edit = new QLineEdit;
    m_middlename_edit = new QLineEdit;
    m_address_edit = new QLineEdit;
    m_birthday_edit = new QLineEdit;
    m_email_edit = new QLineEdit;
    m_phone_edit = new QLineEdit;
    m_phone_list = new QListWidget;

    m_firstname_edit->setPlaceholderText("Required");
    m_surname_edit->setPlaceholderText("Required");
    m_email_edit->setPlaceholderText("Required");
    m_phone_edit->setPlaceholderText("Example: +7(912)345-67-89");
    m_birthday_edit->setPlaceholderText("DD-MM-YYYY (optional)");

    formLayout->addRow("First Name *:", m_firstname_edit);
    formLayout->addRow("Surname *:", m_surname_edit);
    formLayout->addRow("Middle Name:", m_middlename_edit);
    formLayout->addRow("Email *:", m_email_edit);
    formLayout->addRow("Birthday:", m_birthday_edit);
    formLayout->addRow("Address:", m_address_edit);

    QHBoxLayout* phoneLayout = new QHBoxLayout;
    phoneLayout->addWidget(m_phone_edit);
    QPushButton* addPhoneButton = new QPushButton("Add");
    QPushButton* removePhoneButton = new QPushButton("Remove");
    phoneLayout->addWidget(addPhoneButton);
    phoneLayout->addWidget(removePhoneButton);

    formLayout->addRow("Phone *:", phoneLayout);
    formLayout->addRow("Phone Numbers:", m_phone_list);

    mainLayout->addLayout(formLayout);

    QPushButton* okButton = new QPushButton("OK");
    QPushButton* cancelButton = new QPushButton("Cancel");

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    connect(addPhoneButton, &QPushButton::clicked, this, &ContactDialog::on_add_phone);
    connect(removePhoneButton, &QPushButton::clicked, this, &ContactDialog::on_remove_phone);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void ContactDialog::set_contact(const Contact& contact) {
    m_firstname_edit->setText(contact.get_firstname());
    m_surname_edit->setText(contact.get_surname());
    m_middlename_edit->setText(contact.get_middlename());
    m_address_edit->setText(contact.get_address());
    m_birthday_edit->setText(contact.get_birthday());
    m_email_edit->setText(contact.get_email());

    m_phone_list->clear();
    for (const QString& phone : contact.get_phones()) {
        m_phone_list->addItem(phone);
    }
}

Contact ContactDialog::get_contact() const {
    Contact contact;

    contact.set_firstname(m_firstname_edit->text());
    contact.set_surname(m_surname_edit->text());
    contact.set_middlename(m_middlename_edit->text());
    contact.set_address(m_address_edit->text());
    contact.set_birthday(m_birthday_edit->text());
    contact.set_email(m_email_edit->text());

    QStringList phones;
    for (int i = 0; i < m_phone_list->count(); ++i) {
        phones.append(m_phone_list->item(i)->text());
    }
    contact.set_phones(phones);

    return contact;
}

// Добавление телефонного номера с валидацией формата
void ContactDialog::on_add_phone() {
    QString phone = m_phone_edit->text().trimmed();
    if (!phone.isEmpty()) {
        Contact temp;
        if (temp.valid_phone(phone)) {
            QString normalized = temp.normalize_phone(phone);

            bool exists = false;
            for (int i = 0; i < m_phone_list->count(); ++i) {
                if (m_phone_list->item(i)->text() == normalized) {
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                m_phone_list->addItem(normalized);
                m_phone_edit->clear();
            } else {
                QMessageBox::warning(this, "Warning", "This phone number already exists!");
            }
        } else {
            QMessageBox::warning(this, "Invalid Phone",
                                 "Please enter a valid phone number.\n"
                                 "Formats: +78121234567, 88121234567, +7(812)1234567, 8(812)1234567, +7(812)123-45-67, 8(812)123-45-67");
        }
    }
}

// Удаление выбранного телефонного номера из списка
void ContactDialog::on_remove_phone() {
    int row = m_phone_list->currentRow();
    if (row >= 0) {
        delete m_phone_list->takeItem(row);
    }
}

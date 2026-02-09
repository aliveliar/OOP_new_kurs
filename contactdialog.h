#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "contacts.h"

class ContactDialog : public QDialog {
    Q_OBJECT

public:
    ContactDialog(QWidget* parent = nullptr);
    void set_contact(const Contact& contact);
    Contact get_contact() const;

private slots:
    void on_add_phone();
    void on_remove_phone();

private:
    QLineEdit *m_firstname_edit, *m_surname_edit, *m_middlename_edit, *m_address_edit, *m_birthday_edit, *m_email_edit, *m_phone_edit;
    QListWidget *m_phone_list;
};

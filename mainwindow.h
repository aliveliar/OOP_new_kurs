#pragma once
#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QInputDialog>
#include <QApplication>
#include <QGroupBox>
#include <QCheckBox>
#include "contactbook.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_add_contact();
    void on_edit_contact();
    void on_delete_contact();
    void on_search();
    void on_clear_search();
    void on_sort();
    void on_save();
    void on_load();
    void refresh_table();

private:
    void setup_UI();
    void create_menu();
    void show_contact_error(const Contact& contact, bool isEditing = false);

    Contactbook* m_contactbook;
    QTableWidget* m_table;

    QLineEdit* m_search_firstname;
    QLineEdit* m_search_surname;
    QLineEdit* m_search_middlename;
    QLineEdit* m_search_email;
    QLineEdit* m_search_phone;
    QLineEdit* m_search_address;
    QLineEdit* m_search_birthday;

    QPushButton* m_search_button;
    QPushButton* m_clear_search_button;

    QPushButton* m_add_button, *m_edit_button, *m_delete_button, *m_sort_button, *m_save_button, *m_load_button;
};

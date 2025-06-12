#include "CustomerFormPage.h"
#include "DatabaseManager.h"
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QMessageBox>
#include <QSqlQuery>


CustomerFormPage::CustomerFormPage(bool editMode, int customerId, QWidget* parent)
    : QDialog(parent), isEditMode(editMode), customerId(customerId) {
    setWindowTitle(isEditMode ? "Edit Profile" : "Sign Up");
    setModal(true);
    setFixedSize(400, 300);

    setupForm();
    if (isEditMode && customerId >= 0) {
        loadCustomerDetails();
    }
}

void CustomerFormPage::setupForm() {
    nameInput = new QLineEdit(this);
    emailInput = new QLineEdit(this);
    passwordInput = new QLineEdit(this);
    contactInput = new QLineEdit(this);
    passwordInput->setEchoMode(QLineEdit::Password);

    saveButton = new QPushButton(isEditMode ? "Update Profile" : "Register", this);

    QFormLayout* layout = new QFormLayout(this);
    layout->addRow("Name:", nameInput);
    layout->addRow("Email:", emailInput);
    layout->addRow("Password:", passwordInput);
    layout->addRow("Contact:", contactInput);
    layout->addRow(saveButton);

    connect(saveButton, &QPushButton::clicked, this, &CustomerFormPage::saveOrUpdateCustomer);
}

void CustomerFormPage::loadCustomerDetails() {
    QSqlQuery query = DatabaseManager::getUserById(customerId);
    if (query.next()) {
        nameInput->setText(query.value(1).toString());
        emailInput->setText(query.value(2).toString());
        passwordInput->setText(query.value(3).toString());
        contactInput->setText(query.value(5).toString());
        emailInput->setEnabled(false);
    }
}

void CustomerFormPage::saveOrUpdateCustomer() {
    QString name = nameInput->text().trimmed();
    QString email = emailInput->text().trimmed();
    QString password = passwordInput->text().trimmed();
    QString contact = contactInput->text().trimmed();

    if (name.isEmpty() || email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please fill all required fields.");
        return;
    }

    bool result = isEditMode
                      ? DatabaseManager::updateCustomer(customerId, name, password, contact)
                      : DatabaseManager::registerCustomer(name, email, password, contact);

    if (result) {
        QMessageBox::information(this, "Success", isEditMode ? "Profile updated." : "Registration successful.");
        accept();  // ✅ closes the dialog properly
    } else {
        QMessageBox::critical(this, "Failed", isEditMode ? "Update failed." : "Registration failed.");
    }
}

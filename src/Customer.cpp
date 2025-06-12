#include "Customer.h"
#include "MainWindow.h"

Customer::Customer(const QString& name, const QString& email, const QString& password, const QString& contact)
    : User(name, email, password, contact) {}

void Customer::showDashboard(QWidget* parent) {
    auto* label = new QLabel("Customer Dashboard", parent);
    label->setStyleSheet("color: white; font-size: 24px;");
    label->move(100, 100);
    label->show();
}

// Assuming User class has protected or public getters/setters for these fields.
// If not, you need to add them in User class.

QString Customer::getName() const {
    return User::getName(); // or access protected member if available
}

QString Customer::getPassword() const {
    return User::getPassword();
}

QString Customer::getContact() const {
    return User::getContact();
}

void Customer::setName(const QString& newName) {
    User::setName(newName);
}

void Customer::setPassword(const QString& newPassword) {
    User::setPassword(newPassword);
}

void Customer::setContact(const QString& newContact) {
    User::setContact(newContact);
}

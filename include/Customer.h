#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "User.h"
#include <QLabel>

class Customer : public User {
public:
    Customer(const QString& name, const QString& email, const QString& password, const QString& contact);

    void showDashboard(QWidget* parent) override;

    // Add getters and setters that call User's protected/private members or methods
    QString getName() const;
    QString getPassword() const;
    QString getContact() const;

    void setName(const QString& newName);
    void setPassword(const QString& newPassword);
    void setContact(const QString& newContact);
};

#endif

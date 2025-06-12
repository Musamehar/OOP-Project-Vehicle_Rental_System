#ifndef USER_H
#define USER_H

#include <QString>
#include <QWidget>

class User {
protected:
    QString name, email, password, contact;
    int id;
    QString billingAddress;
public:
    User(const QString& name, const QString& email, const QString& password, const QString& contact);
    virtual ~User() {}

    // Getters
    QString getName() const;
    QString getEmail() const;
    QString getPassword() const;
    QString getContact() const;
    QString getBillingAddress() const { return billingAddress; }

    // Setters - ADD THESE
    void setName(const QString& newName);
    void setEmail(const QString& newEmail);
    void setPassword(const QString& newPassword);
    void setContact(const QString& newContact);
    void setBillingAddress(const QString& address) { billingAddress = address; }
    void setId(int userId) { id = userId; }
    int getId() const { return id; }

    virtual void showDashboard(QWidget* parent) = 0;
};

#endif // USER_H

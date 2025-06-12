#include "User.h"

// Constructor
User::User(const QString& name, const QString& email, const QString& password, const QString& contact)
    : name(name), email(email), password(password), contact(contact) {}

// Getters
QString User::getName() const { return name; }
QString User::getEmail() const { return email; }
QString User::getPassword() const { return password; }
QString User::getContact() const { return contact; }

// Setters
void User::setName(const QString& newName) { name = newName; }
void User::setEmail(const QString& newEmail) { email = newEmail; }
void User::setPassword(const QString& newPassword) { password = newPassword; }
void User::setContact(const QString& newContact) { contact = newContact; }

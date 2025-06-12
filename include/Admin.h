#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include <QLabel>

class Admin : public User {
public:
    Admin(const QString& name, const QString& email, const QString& password, const QString& contact);
    void showDashboard(QWidget* parent) override;
};

#endif

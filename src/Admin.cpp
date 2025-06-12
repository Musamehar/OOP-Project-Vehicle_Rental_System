#include "Admin.h"

Admin::Admin(const QString& name, const QString& email, const QString& password, const QString& contact)
    : User(name, email, password, contact) {}

void Admin::showDashboard(QWidget* parent) {
    auto* label = new QLabel("Admin Dashboard", parent);
    label->setStyleSheet("color: white; font-size: 24px;");
    label->move(100, 100);
    label->show();
}

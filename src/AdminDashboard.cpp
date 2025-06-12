#include "AdminDashboard.h"
#include "DatabaseManager.h"
#include <QVBoxLayout>
#include <QFont>

AdminDashboard::AdminDashboard(QWidget* parent) : QWidget(parent) {
    revenueLabel = new QLabel(this);
    activeBookingsLabel = new QLabel(this);
    vehiclesInUseLabel = new QLabel(this);

    QFont font;
    font.setPointSize(14);
    font.setBold(true);

    revenueLabel->setFont(font);
    activeBookingsLabel->setFont(font);
    vehiclesInUseLabel->setFont(font);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(revenueLabel);
    layout->addWidget(activeBookingsLabel);
    layout->addWidget(vehiclesInUseLabel);

    loadSummary();
}

void AdminDashboard::loadSummary() {
    double revenue = DatabaseManager::getTotalRevenue();
    int active = DatabaseManager::getActiveBookingCount();
    int inUse = DatabaseManager::getVehiclesInUseCount();

    revenueLabel->setText(QString("💰 Total Revenue: $%1").arg(revenue, 0, 'f', 2));
    activeBookingsLabel->setText(QString("📅 Active Bookings: %1").arg(active));
    vehiclesInUseLabel->setText(QString("🚗 Vehicles in Use: %1").arg(inUse));
}

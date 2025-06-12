#include "MainWindow.h"
#include "AdminDashboard.h"
#include "VehicleManagementPage.h"
#include "ReportsPage.h"
#include "BookingPage.h"
#include "CustomerFormPage.h"
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>

MainWindow::MainWindow(User* user, QWidget* parent)
    : QMainWindow(parent), currentUser(user) {
    setWindowTitle("Royal Car Rentals - Dashboard");

    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    setupUI();
    setupNavigation();
}

void MainWindow::setupUI() {
    // Always loaded
    dashboardPage = new AdminDashboard(this);
    bookingsPage = new BookingPage(1, this); // Assume customer ID = 1 for demo
    vehiclesPage = new VehicleManagementPage(this);
    reportsPage = new ReportsPage(this);
    customerFormPage = new CustomerFormPage(true, 1, this); // Assume customer ID = 1

    stack->addWidget(dashboardPage);     // index 0
    stack->addWidget(vehiclesPage);      // index 1
    stack->addWidget(bookingsPage);      // index 2
    stack->addWidget(reportsPage);       // index 3
    stack->addWidget(customerFormPage);  // index 4

    stack->setCurrentIndex(0);
}

void MainWindow::setupNavigation() {
    QToolBar* toolbar = addToolBar("Navigation");

    QAction* dashboardAction = toolbar->addAction("Dashboard");
    QAction* vehiclesAction = toolbar->addAction("Vehicles");
    QAction* bookingsAction = toolbar->addAction("Bookings");
    QAction* reportsAction = toolbar->addAction("Reports");
    QAction* profileAction = toolbar->addAction("Profile");

    connect(dashboardAction, &QAction::triggered, this, [=]() {
        stack->setCurrentWidget(dashboardPage);
    });
    connect(vehiclesAction, &QAction::triggered, this, [=]() {
        stack->setCurrentWidget(vehiclesPage);
    });
    connect(bookingsAction, &QAction::triggered, this, [=]() {
        stack->setCurrentWidget(bookingsPage);
    });
    connect(reportsAction, &QAction::triggered, this, [=]() {
        stack->setCurrentWidget(reportsPage);
    });
    connect(profileAction, &QAction::triggered, this, [=]() {
        stack->setCurrentWidget(customerFormPage);
    });
}

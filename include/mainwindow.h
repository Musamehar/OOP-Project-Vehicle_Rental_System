#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QToolBar>
#include "User.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

    QStackedWidget* stack;
    QWidget* dashboardPage;
    QWidget* bookingsPage;
    QWidget* vehiclesPage;
    QWidget* reportsPage;
    QWidget* customerFormPage;

    void setupUI();
    void setupNavigation();

    User* currentUser;

public:
    explicit MainWindow(User* user, QWidget* parent = nullptr);
};

#endif

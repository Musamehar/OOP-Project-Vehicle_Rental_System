#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QWidget>
#include <QLabel>

class AdminDashboard : public QWidget {
    Q_OBJECT

    QLabel *revenueLabel;
    QLabel *activeBookingsLabel;
    QLabel *vehiclesInUseLabel;

    void loadSummary();

public:
    AdminDashboard(QWidget* parent = nullptr);
};

#endif

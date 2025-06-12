#ifndef VEHICLEMANAGEMENTPAGE_H
#define VEHICLEMANAGEMENTPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>

class VehicleManagementPage : public QWidget {
    Q_OBJECT

    QLineEdit *brandInput, *modelInput, *rateInput;
    QPushButton *addButton, *deleteButton;
    QTableWidget *table;

    void loadVehicles();

public:
    VehicleManagementPage(QWidget *parent = nullptr);

private slots:
    void addVehicle();
    void deleteSelectedVehicle();
};

#endif

#ifndef BOOKINGPAGE_H
#define BOOKINGPAGE_H

#include <QWidget>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
#include <QMap>

class BookingPage : public QWidget {
    Q_OBJECT

    QComboBox *vehicleDropdown;
    QDateEdit *startDateEdit, *endDateEdit;
    QLabel *totalLabel;
    QPushButton *bookButton;

    QMap<QString, QPair<int, double>> vehicleData; // name → (vehicle_id, rate)
    int customerId;

    void loadAvailableVehicles();
    void calculateTotal();

public:
    BookingPage(int customerId, QWidget* parent = nullptr);

private slots:
    void updateTotal();
    void confirmBooking();
};

#endif

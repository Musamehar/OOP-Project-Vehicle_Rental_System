#ifndef BOOKING_H
#define BOOKING_H

#include <QString>
#include <QDate>

class Booking {
private:
    int customerId, vehicleId;
    QDate startDate, endDate;
    double totalAmount;

public:
    Booking(int customerId, int vehicleId, QDate start, QDate end, double amount);

    int getCustomerId() const;
    int getVehicleId() const;
    QDate getStartDate() const;
    QDate getEndDate() const;
    double getTotalAmount() const;
};

#endif

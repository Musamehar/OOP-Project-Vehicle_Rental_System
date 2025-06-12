#include "Booking.h"

Booking::Booking(int customerId, int vehicleId, QDate start, QDate end, double amount)
    : customerId(customerId), vehicleId(vehicleId), startDate(start), endDate(end), totalAmount(amount) {}

int Booking::getCustomerId() const { return customerId; }
int Booking::getVehicleId() const { return vehicleId; }
QDate Booking::getStartDate() const { return startDate; }
QDate Booking::getEndDate() const { return endDate; }
double Booking::getTotalAmount() const { return totalAmount; }

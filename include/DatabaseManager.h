#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>

class Booking;

class DatabaseManager {
public:
    static bool connect();
    static QSqlQuery getUserByEmail(const QString& email);

    static void runSeedIfNeeded();

    static void ensureFallbackAdmin();

    static bool addVehicle(const QString& brand, const QString& model, double rate);
    static QSqlQuery getAllVehicles();
    static bool deleteVehicle(int id);

    static QSqlQuery getAvailableVehicles();
    static bool insertBooking(const Booking& booking);

    static QSqlQuery getActiveBookings(int customerId);
    static bool processReturn(int bookingId, int vehicleId, const QDate& returnDate, double penalty);

    static bool registerCustomer(const QString& name, const QString& email, const QString& password, const QString& contact);
    static bool updateCustomer(int id, const QString& name, const QString& password, const QString& contact);
    static QSqlQuery getUserById(int id);

    static QSqlQuery getInvoiceDetails(int bookingId);

    static QSqlQuery getRevenuePerVehicle();
    static QSqlQuery getMonthlyProfitLoss();

    static double getTotalRevenue();
    static int getActiveBookingCount();
    static int getVehiclesInUseCount();

    static void printAllUsers();  // ✅ add this line
};


#endif

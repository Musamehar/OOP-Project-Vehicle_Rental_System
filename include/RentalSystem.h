#ifndef RENTALSYSTEM_H
#define RENTALSYSTEM_H

#include <QObject>
#include <QList>
#include "User.h"
#include "Vehicle.h"
#include "Booking.h"

class RentalSystem : public QObject {
    Q_OBJECT

private:
    QList<User*> users;
    QList<Vehicle*> vehicles;
    QList<Booking*> bookings;

    static RentalSystem* instance;

    explicit RentalSystem(QObject* parent = nullptr); // singleton

public:
    ~RentalSystem();

    static RentalSystem* getInstance();

    void loadUsers();
    void loadVehicles();
    void loadBookings();
    void addUser(User* user);
    void addVehicle(Vehicle* vehicle);


    QList<User*> getAllUsers() const;
    QList<Vehicle*> getAllVehicles() const;
    QList<Booking*> getAllBookings() const;

    QList<Vehicle*> getAvailableVehicles() const;
    User* findUserByEmail(const QString& email) const;

    void addBooking(const Booking& booking);
};

#endif

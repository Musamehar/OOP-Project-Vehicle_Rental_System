#include "RentalSystem.h"
#include "DatabaseManager.h"
#include "Admin.h"
#include "Customer.h"
#include "Car.h"

RentalSystem* RentalSystem::instance = nullptr;

RentalSystem::RentalSystem(QObject* parent) : QObject(parent) {
    loadUsers();
    loadVehicles();
    loadBookings();
}

RentalSystem::~RentalSystem() {
    qDeleteAll(users);
    qDeleteAll(vehicles);
    qDeleteAll(bookings);
}

RentalSystem* RentalSystem::getInstance() {
    if (!instance)
        instance = new RentalSystem();
    return instance;
}

void RentalSystem::loadUsers() {
    QSqlQuery query("SELECT id, name, email, password, role, contact FROM users");
    while (query.next()) {
        QString role = query.value(4).toString();
        User* user = nullptr;

        if (role == "admin") {
            user = new Admin(query.value(1).toString(), query.value(2).toString(),
                             query.value(3).toString(), query.value(5).toString());
        } else {
            user = new Customer(query.value(1).toString(), query.value(2).toString(),
                                query.value(3).toString(), query.value(5).toString());
        }

        users.append(user);
    }
}

void RentalSystem::loadVehicles() {
    QSqlQuery query("SELECT id, brand, model, type, rate, available FROM vehicles");
    while (query.next()) {
        Vehicle* v = new Car(query.value(0).toInt(), query.value(1).toString(),
                             query.value(2).toString(), query.value(4).toDouble(),
                             query.value(5).toBool());
        vehicles.append(v);
    }
}

void RentalSystem::loadBookings() {
    QSqlQuery query("SELECT customer_id, vehicle_id, start_date, end_date, total_amount FROM bookings");
    while (query.next()) {
        Booking* b = new Booking(query.value(0).toInt(), query.value(1).toInt(),
                                 QDate::fromString(query.value(2).toString(), "yyyy-MM-dd"),
                                 QDate::fromString(query.value(3).toString(), "yyyy-MM-dd"),
                                 query.value(4).toDouble());
        bookings.append(b);
    }
}

QList<User*> RentalSystem::getAllUsers() const {
    return users;
}

QList<Vehicle*> RentalSystem::getAllVehicles() const {
    return vehicles;
}

QList<Booking*> RentalSystem::getAllBookings() const {
    return bookings;
}

QList<Vehicle*> RentalSystem::getAvailableVehicles() const {
    QList<Vehicle*> available;
    for (Vehicle* v : vehicles)
        if (v->isAvailable())
            available.append(v);
    return available;
}

User* RentalSystem::findUserByEmail(const QString& email) const {
    for (User* u : users)
        if (u->getEmail() == email)
            return u;
    return nullptr;
}

void RentalSystem::addBooking(const Booking& booking) {
    bookings.append(new Booking(booking));
    DatabaseManager::insertBooking(booking);
}

void RentalSystem::addUser(User* user) {
    users.append(user);
    auto* customer = dynamic_cast<Customer*>(user);
    if (customer) {
        DatabaseManager::registerCustomer(customer->getName(), customer->getEmail(),
                                          customer->getPassword(), customer->getContact());
    }
}

void RentalSystem::addVehicle(Vehicle* vehicle) {
    vehicles.append(vehicle);
    auto* car = dynamic_cast<Car*>(vehicle);
    if (car) {
        DatabaseManager::addVehicle(car->getBrand(), car->getModel(), car->getRate());
    }
}

#include "DatabaseManager.h"
#include "Booking.h"
#include <QFile>
#include <QTextStream>
#include <QSqlQuery>

bool DatabaseManager::connect() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString path = "C:/Users/AL RASHIDS/Documents/RoyalCarRental/data/royal_car_rental.db";

    db.setDatabaseName(path);

    if (!db.open()) {
        qWarning() << "❌ DB Connection Failed:" << db.lastError().text();
        return false;
    }

    qDebug() << "✅ Connected to SQLite:" << path;
    return true;
}


void DatabaseManager::runSeedIfNeeded() {
    QSqlQuery query;
    query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='users'");
    if (query.next()) {
        qDebug() << "✅ Database already initialized.";
        return;
    }

    qDebug() << "⚠️ No tables found. Running seed.sql...";
    QString path = "C:/Users/AL RASHIDS/Documents/RoyalCarRental/data/seed.sql";
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "❌ Failed to open seed.sql";
        return;
    }

    QTextStream in(&file);
    QStringList statements = in.readAll().split(';', Qt::SkipEmptyParts);
    for (QString& stmt : statements) {
        stmt = stmt.trimmed();
        if (!stmt.isEmpty()) {
            QSqlQuery q;
            if (!q.exec(stmt)) {
                qWarning() << "❌ Failed to exec:" << stmt << "→" << q.lastError().text();
            }
        }
    }

    file.close();
    qDebug() << "✅ Seed completed.";
}

QSqlQuery DatabaseManager::getUserByEmail(const QString& email) {
    QSqlQuery query;
    query.prepare("SELECT id, name, email, password, contact, role FROM users WHERE email = ?");
    query.addBindValue(email);

    if (!query.exec()) {
        qWarning() << "❌ Query failed:" << query.lastError().text();
        return query;
    }

    return query; // DO NOT call query.next() here
}


bool DatabaseManager::addVehicle(const QString& brand, const QString& model, double rate) {
    QSqlQuery query;
    query.prepare("INSERT INTO vehicles (brand, model, type, rate, available) VALUES (?, ?, 'Car', ?, 1)");
    query.addBindValue(brand);
    query.addBindValue(model);
    query.addBindValue(rate);
    return query.exec();
}

QSqlQuery DatabaseManager::getAllVehicles() {
    QSqlQuery query("SELECT id, brand, model, rate, available FROM vehicles WHERE type='Car'");
    return query;
}

bool DatabaseManager::deleteVehicle(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM vehicles WHERE id = ?");
    query.addBindValue(id);
    return query.exec();
}

QSqlQuery DatabaseManager::getAvailableVehicles() {
    QSqlQuery query("SELECT id, brand || ' ' || model, rate FROM vehicles WHERE available = 1");
    return query;
}

bool DatabaseManager::insertBooking(const Booking& booking) {
    QSqlQuery query;
    query.prepare("INSERT INTO bookings (customer_id, vehicle_id, start_date, end_date, total_amount) "
                  "VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(booking.getCustomerId());
    query.addBindValue(booking.getVehicleId());
    query.addBindValue(booking.getStartDate().toString("yyyy-MM-dd"));
    query.addBindValue(booking.getEndDate().toString("yyyy-MM-dd"));
    query.addBindValue(booking.getTotalAmount());
    return query.exec();
}

QSqlQuery DatabaseManager::getActiveBookings(int customerId) {
    QSqlQuery query;
    query.prepare(R"(
        SELECT b.id, v.brand || ' ' || v.model, b.end_date, v.rate, v.id
        FROM bookings b
        JOIN vehicles v ON b.vehicle_id = v.id
        WHERE b.customer_id = ? AND b.id NOT IN (SELECT booking_id FROM returns)
    )");
    query.addBindValue(customerId);
    query.exec();
    return query;
}

bool DatabaseManager::processReturn(int bookingId, int vehicleId, const QDate& returnDate, double penalty) {
    QSqlQuery query;
    query.prepare("INSERT INTO returns (booking_id, return_date, penalty_fee) VALUES (?, ?, ?)");
    query.addBindValue(bookingId);
    query.addBindValue(returnDate.toString("yyyy-MM-dd"));
    query.addBindValue(penalty);

    if (!query.exec()) return false;

    QSqlQuery update;
    update.prepare("UPDATE vehicles SET available = 1 WHERE id = ?");
    update.addBindValue(vehicleId);
    return update.exec();
}

bool DatabaseManager::registerCustomer(const QString& name, const QString& email,
                                       const QString& password, const QString& contact) {
    QSqlDatabase db = QSqlDatabase::database();

    // Start transaction
    if (!db.transaction()) {
        qWarning() << "❌ Failed to start transaction:" << db.lastError().text();
        return false;
    }

    // Check if email exists
    QSqlQuery check;
    check.prepare("SELECT id FROM users WHERE email = ?");
    check.addBindValue(email);

    if (!check.exec()) {
        qWarning() << "❌ Email check failed:" << check.lastError().text();
        db.rollback();
        return false;
    }

    if (check.next()) {
        qWarning() << "❌ Email already exists:" << email;
        db.rollback();
        return false;
    }

    // Insert new user
    QSqlQuery query;
    query.prepare("INSERT INTO users (name, email, password, role, contact) VALUES (?, ?, ?, 'customer', ?)");
    query.addBindValue(name);
    query.addBindValue(email);
    query.addBindValue(password);
    query.addBindValue(contact);

    if (!query.exec()) {
        qWarning() << "❌ Registration failed:" << query.lastError().text();
        db.rollback();
        return false;
    }

    // Verify the insert was successful
    QSqlQuery verify;
    verify.prepare("SELECT COUNT(*) FROM users WHERE email = ?");
    verify.addBindValue(email);

    if (!verify.exec() || !verify.next()) {
        qWarning() << "❌ Verification query failed:" << verify.lastError().text();
        db.rollback();
        return false;
    }

    if (verify.value(0).toInt() == 0) {
        qWarning() << "❌ User not found after insertion";
        db.rollback();
        return false;
    }

    // Commit transaction
    if (!db.commit()) {
        qWarning() << "❌ Commit failed:" << db.lastError().text();
        db.rollback();
        return false;
    }

    qDebug() << "✅ Successfully registered customer:" << email;
    printAllUsers();  // Debug output to verify all users in DB
    return true;
}

bool DatabaseManager::updateCustomer(int id, const QString& name, const QString& password, const QString& contact) {
    QSqlQuery query;
    query.prepare("UPDATE users SET name = ?, password = ?, contact = ? WHERE id = ?");
    query.addBindValue(name);
    query.addBindValue(password);
    query.addBindValue(contact);
    query.addBindValue(id);
    return query.exec();
}

QSqlQuery DatabaseManager::getUserById(int id) {
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE id = ?");
    query.addBindValue(id);
    query.exec();
    return query;
}

QSqlQuery DatabaseManager::getInvoiceDetails(int bookingId) {
    QSqlQuery query;
    query.prepare(R"(
        SELECT u.name AS customer,
               v.brand || ' ' || v.model AS vehicle,
               b.start_date, b.end_date, b.total_amount,
               r.return_date, r.penalty_fee
        FROM bookings b
        JOIN users u ON b.customer_id = u.id
        JOIN vehicles v ON b.vehicle_id = v.id
        JOIN returns r ON b.id = r.booking_id
        WHERE b.id = ?
    )");
    query.addBindValue(bookingId);
    query.exec();
    return query;
}

QSqlQuery DatabaseManager::getRevenuePerVehicle() {
    QSqlQuery query;
    query.exec(R"(
        SELECT v.brand || ' ' || v.model AS vehicle, SUM(b.total_amount)
        FROM bookings b
        JOIN vehicles v ON b.vehicle_id = v.id
        GROUP BY v.id
    )");
    return query;
}

QSqlQuery DatabaseManager::getMonthlyProfitLoss() {
    QSqlQuery query;
    query.exec(R"(
        SELECT strftime('%Y-%m', b.start_date) AS month,
               SUM(b.total_amount - IFNULL(r.penalty_fee, 0)) AS profit
        FROM bookings b
        LEFT JOIN returns r ON b.id = r.booking_id
        GROUP BY month
        ORDER BY month
    )");
    return query;
}

double DatabaseManager::getTotalRevenue() {
    QSqlQuery query("SELECT SUM(total_amount) FROM bookings");
    return query.next() ? query.value(0).toDouble() : 0.0;
}

int DatabaseManager::getActiveBookingCount() {
    QSqlQuery query("SELECT COUNT(*) FROM bookings WHERE id NOT IN (SELECT booking_id FROM returns)");
    return query.next() ? query.value(0).toInt() : 0;
}

int DatabaseManager::getVehiclesInUseCount() {
    QSqlQuery query("SELECT COUNT(*) FROM vehicles WHERE available = 0");
    return query.next() ? query.value(0).toInt() : 0;
}

void DatabaseManager::ensureFallbackAdmin() {
    QSqlQuery check;
    check.prepare("SELECT COUNT(*) FROM users WHERE role = 'admin'");
    if (check.exec() && check.next()) {
        int count = check.value(0).toInt();
        if (count == 0) {
            QSqlQuery insert;
            insert.prepare(R"(
                INSERT INTO users (name, email, password, role, contact)
                VALUES ('Admin', 'admin@royal.com', 'admin123', 'admin', '0001112222')
            )");
            insert.exec();
        }
    }
}

// Optional Debug Tool
void DatabaseManager::printAllUsers() {
    QSqlQuery query("SELECT name, email, role FROM users");
    qDebug() << "📋 Users in DB:";
    while (query.next()) {
        qDebug() << "•" << query.value("name").toString()
            << "|" << query.value("email").toString()
            << "|" << query.value("role").toString();
    }
}


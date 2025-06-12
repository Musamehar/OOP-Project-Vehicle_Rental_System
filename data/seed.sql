
-- Sample seed for RoyalCarRental DB
DROP TABLE IF EXISTS users;
DROP TABLE IF EXISTS vehicles;
DROP TABLE IF EXISTS bookings;
DROP TABLE IF EXISTS returns;

CREATE TABLE users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT, email TEXT UNIQUE, password TEXT,
    role TEXT CHECK(role IN ('admin', 'customer')), contact TEXT
);

CREATE TABLE vehicles (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    brand TEXT, model TEXT, type TEXT,
    rate REAL, available INTEGER
);

CREATE TABLE bookings (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    customer_id INTEGER, vehicle_id INTEGER,
    start_date TEXT, end_date TEXT,
    total_amount REAL
);

CREATE TABLE returns (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    booking_id INTEGER, return_date TEXT, penalty_fee REAL
);

-- Dummy Admin
INSERT INTO users (name, email, password, role, contact)
VALUES ('Admin', 'admin@royal.com', 'admin123', 'admin', '0001112222');

-- Dummy Customers
INSERT INTO users (name, email, password, role, contact)
VALUES 
('Alice', 'alice@example.com', 'alice123', 'customer', '1234567890'),
('Bob', 'bob@example.com', 'bob123', 'customer', '0987654321');

-- Dummy Vehicles
INSERT INTO vehicles (brand, model, type, rate, available)
VALUES 
('Toyota', 'Corolla', 'Car', 50.0, 1),
('Honda', 'Civic', 'Car', 55.0, 1),
('Tesla', 'Model 3', 'Car', 100.0, 1);

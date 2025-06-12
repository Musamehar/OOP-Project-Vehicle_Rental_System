#include "BookingPage.h"
#include "DatabaseManager.h"
#include "Booking.h"
#include <QVBoxLayout>
#include <QMessageBox>

BookingPage::BookingPage(int customerId, QWidget* parent)
    : QWidget(parent), customerId(customerId) {
    vehicleDropdown = new QComboBox(this);
    startDateEdit = new QDateEdit(QDate::currentDate(), this);
    endDateEdit = new QDateEdit(QDate::currentDate().addDays(1), this);
    totalLabel = new QLabel("Total: $0.00", this);
    bookButton = new QPushButton("Book Vehicle", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(vehicleDropdown);
    layout->addWidget(startDateEdit);
    layout->addWidget(endDateEdit);
    layout->addWidget(totalLabel);
    layout->addWidget(bookButton);

    loadAvailableVehicles();
    calculateTotal();

    connect(startDateEdit, &QDateEdit::dateChanged, this, &BookingPage::updateTotal);
    connect(endDateEdit, &QDateEdit::dateChanged, this, &BookingPage::updateTotal);
    connect(vehicleDropdown, &QComboBox::currentTextChanged, this, &BookingPage::updateTotal);
    connect(bookButton, &QPushButton::clicked, this, &BookingPage::confirmBooking);
}

void BookingPage::loadAvailableVehicles() {
    QSqlQuery query = DatabaseManager::getAvailableVehicles();
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        double rate = query.value(2).toDouble();
        vehicleDropdown->addItem(name);
        vehicleData[name] = qMakePair(id, rate);
    }
}

void BookingPage::calculateTotal() {
    QString selected = vehicleDropdown->currentText();
    if (selected.isEmpty()) return;

    QDate start = startDateEdit->date();
    QDate end = endDateEdit->date();
    int days = start.daysTo(end);
    if (days <= 0) days = 1;

    double rate = vehicleData[selected].second;
    double total = rate * days;
    totalLabel->setText(QString("Total: $%1").arg(total, 0, 'f', 2));
}

void BookingPage::updateTotal() {
    calculateTotal();
}

void BookingPage::confirmBooking() {
    QString selected = vehicleDropdown->currentText();
    if (selected.isEmpty()) return;

    int vehicleId = vehicleData[selected].first;
    QDate start = startDateEdit->date();
    QDate end = endDateEdit->date();
    double rate = vehicleData[selected].second;
    int days = start.daysTo(end);
    if (days <= 0) days = 1;
    double total = rate * days;

    Booking booking(customerId, vehicleId, start, end, total);
    if (DatabaseManager::insertBooking(booking)) {
        QMessageBox::information(this, "Booked", "Vehicle booked successfully.");
    } else {
        QMessageBox::critical(this, "Error", "Booking failed.");
    }
}

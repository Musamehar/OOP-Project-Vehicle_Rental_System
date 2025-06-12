#include "ReturnPage.h"
#include "DatabaseManager.h"
#include <QVBoxLayout>
#include <QMessageBox>

ReturnPage::ReturnPage(int customerId, QWidget* parent)
    : QWidget(parent), customerId(customerId) {
    bookingDropdown = new QComboBox(this);
    returnDateEdit = new QDateEdit(QDate::currentDate(), this);
    penaltyLabel = new QLabel("Penalty: $0.00", this);
    returnButton = new QPushButton("Return Vehicle", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(bookingDropdown);
    layout->addWidget(returnDateEdit);
    layout->addWidget(penaltyLabel);
    layout->addWidget(returnButton);

    loadBookings();
    calculatePenalty();

    connect(bookingDropdown, &QComboBox::currentTextChanged, this, &ReturnPage::onBookingSelected);
    connect(returnDateEdit, &QDateEdit::dateChanged, this, &ReturnPage::calculatePenalty);
    connect(returnButton, &QPushButton::clicked, this, &ReturnPage::processReturn);
}

void ReturnPage::loadBookings() {
    QSqlQuery query = DatabaseManager::getActiveBookings(customerId);
    while (query.next()) {
        int id = query.value(0).toInt();
        QString vehicleName = query.value(1).toString();
        QDate endDate = QDate::fromString(query.value(2).toString(), "yyyy-MM-dd");
        double rate = query.value(3).toDouble();
        int vehicleId = query.value(4).toInt();

        QString display = QString("#%1 - %2 (Due: %3)").arg(id).arg(vehicleName).arg(endDate.toString("dd MMM"));
        bookingDropdown->addItem(display);
        bookingMap[display] = { id, vehicleId, endDate, rate };
    }
}

void ReturnPage::calculatePenalty() {
    QString selected = bookingDropdown->currentText();
    if (!bookingMap.contains(selected)) return;

    QDate actualReturn = returnDateEdit->date();
    QDate due = bookingMap[selected].dueDate;
    double rate = bookingMap[selected].rate;

    int lateDays = due.daysTo(actualReturn);
    double penalty = (lateDays > 0) ? lateDays * (rate * 0.5) : 0;

    penaltyLabel->setText(QString("Penalty: $%1").arg(penalty, 0, 'f', 2));
}

void ReturnPage::onBookingSelected() {
    calculatePenalty();
}

void ReturnPage::processReturn() {
    QString selected = bookingDropdown->currentText();
    if (!bookingMap.contains(selected)) return;

    BookingInfo info = bookingMap[selected];
    QDate actualReturn = returnDateEdit->date();
    QDate due = info.dueDate;
    double rate = info.rate;
    int lateDays = due.daysTo(actualReturn);
    double penalty = (lateDays > 0) ? lateDays * (rate * 0.5) : 0;

    if (DatabaseManager::processReturn(info.bookingId, info.vehicleId, actualReturn, penalty)) {
        QMessageBox::information(this, "Returned", "Vehicle returned successfully.");
        bookingDropdown->removeItem(bookingDropdown->currentIndex());
    } else {
        QMessageBox::critical(this, "Error", "Return failed.");
    }
}

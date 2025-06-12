#ifndef RETURNPAGE_H
#define RETURNPAGE_H

#include <QWidget>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
#include <QMap>

class ReturnPage : public QWidget {
    Q_OBJECT

    QComboBox *bookingDropdown;
    QDateEdit *returnDateEdit;
    QLabel *penaltyLabel;
    QPushButton *returnButton;

    struct BookingInfo {
        int bookingId;
        int vehicleId;
        QDate dueDate;
        double rate;
    };

    QMap<QString, BookingInfo> bookingMap;
    int customerId;

    void loadBookings();
    void calculatePenalty();

public:
    ReturnPage(int customerId, QWidget* parent = nullptr);

private slots:
    void onBookingSelected();
    void processReturn();
};

#endif

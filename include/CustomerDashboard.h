#ifndef CUSTOMERDASHBOARD_H
#define CUSTOMERDASHBOARD_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QButtonGroup>
#include <QTimeEdit>
#include <QDialog>
#include "Customer.h"

struct CarData {
    int id;
    QString name;
    QString type;
    QString image; // Path to image file
    int capacity;
    QString transmission;
    double pricePerDay;
    double rating;
    bool isAvailable;
};

class CustomerDashboard : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerDashboard(Customer* customer, QWidget* loginWindow, QWidget* parent = nullptr);
private slots:
    void showHomePage();
    void showBookingPage();
    void showAboutPage();
    void showTermsPage();
    void showSettingsPage();
    void searchCars();
    void filterCars();
    void logout();
    void handleRentClicked(const CarData& car);

private:
    QWidget* createHomePage();
    Customer* currentCustomer;
    QWidget* loginRef;
    QStackedWidget* contentStack;

    // Filter controls
    QButtonGroup* carTypeGroup;
    QButtonGroup* capacityGroup;
    QButtonGroup* recommendationGroup;

    // Search controls
    QComboBox* pickupLocation;
    QComboBox* dropoffLocation;
    QDateEdit* pickupDate;
    QDateEdit* dropoffDate;
    QTimeEdit* pickupTime;
    QTimeEdit* dropoffTime;

    QWidget* carContainer;
    QList<CarData> allCars;
    QList<CarData> filteredCars;

    QWidget* createBookingPage();
    QWidget* createAboutPage();
    QWidget* createTermsPage();
    QWidget* createSettingsPage();
    QWidget* createCarCard(const CarData& car);
    QWidget* createFilterSection();
    QWidget* createSearchSection();

    void styleNavbar(QPushButton* btn);
    void loadCarsFromDatabase();
    void displayCars(const QList<CarData>& cars);
    void applyFilters();

    void showRentDialog(const CarData& car);
};

#endif

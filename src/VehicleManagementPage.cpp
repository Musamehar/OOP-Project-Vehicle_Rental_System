#include "VehicleManagementPage.h"
#include "DatabaseManager.h"
#include <QVBoxLayout>
#include <QMessageBox>

VehicleManagementPage::VehicleManagementPage(QWidget *parent) : QWidget(parent) {
    brandInput = new QLineEdit(this); brandInput->setPlaceholderText("Brand");
    modelInput = new QLineEdit(this); modelInput->setPlaceholderText("Model");
    rateInput = new QLineEdit(this); rateInput->setPlaceholderText("Rate");
    addButton = new QPushButton("Add Vehicle", this);
    deleteButton = new QPushButton("Delete Selected", this);
    table = new QTableWidget(this);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"ID", "Brand", "Model", "Rate", "Available"});

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(brandInput);
    layout->addWidget(modelInput);
    layout->addWidget(rateInput);
    layout->addWidget(addButton);
    layout->addWidget(deleteButton);
    layout->addWidget(table);

    connect(addButton, &QPushButton::clicked, this, &VehicleManagementPage::addVehicle);
    connect(deleteButton, &QPushButton::clicked, this, &VehicleManagementPage::deleteSelectedVehicle);

    loadVehicles();
}

void VehicleManagementPage::loadVehicles() {
    table->setRowCount(0);
    QSqlQuery query = DatabaseManager::getAllVehicles();
    int row = 0;
    while (query.next()) {
        table->insertRow(row);
        for (int col = 0; col < 5; ++col) {
            table->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        ++row;
    }
}

void VehicleManagementPage::addVehicle() {
    QString brand = brandInput->text();
    QString model = modelInput->text();
    double rate = rateInput->text().toDouble();

    if (brand.isEmpty() || model.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Brand and Model required.");
        return;
    }

    if (DatabaseManager::addVehicle(brand, model, rate)) {
        QMessageBox::information(this, "Success", "Vehicle added.");
        loadVehicles();
    } else {
        QMessageBox::critical(this, "Error", "Failed to add vehicle.");
    }
}

void VehicleManagementPage::deleteSelectedVehicle() {
    int row = table->currentRow();
    if (row < 0) return;

    int id = table->item(row, 0)->text().toInt();
    if (DatabaseManager::deleteVehicle(id)) {
        table->removeRow(row);
    } else {
        QMessageBox::warning(this, "Error", "Deletion failed.");
    }
}

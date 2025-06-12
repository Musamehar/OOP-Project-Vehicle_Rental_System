#include "Vehicle.h"

Vehicle::Vehicle(int id, const QString& brand, const QString& model, const QString& type, double rate, bool available)
    : id(id), brand(brand), model(model), type(type), rate(rate), available(available) {}

int Vehicle::getId() const { return id; }
QString Vehicle::getBrand() const { return brand; }
QString Vehicle::getModel() const { return model; }
QString Vehicle::getType() const { return type; }
double Vehicle::getRate() const { return rate; }
bool Vehicle::isAvailable() const { return available; }
void Vehicle::setAvailable(bool status) { available = status; }

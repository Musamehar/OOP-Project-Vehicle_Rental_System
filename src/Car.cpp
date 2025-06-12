#include "Car.h"

Car::Car(int id, const QString& brand, const QString& model, double rate, bool available)
    : Vehicle(id, brand, model, "Car", rate, available) {}

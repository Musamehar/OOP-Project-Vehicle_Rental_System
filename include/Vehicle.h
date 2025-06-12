#ifndef VEHICLE_H
#define VEHICLE_H

#include <QString>

class Vehicle {
protected:
    int id;
    QString brand, model, type;
    double rate;
    bool available;

public:
    Vehicle(int id, const QString& brand, const QString& model, const QString& type, double rate, bool available);
    virtual ~Vehicle() {}

    int getId() const;
    QString getBrand() const;
    QString getModel() const;
    QString getType() const;
    double getRate() const;
    bool isAvailable() const;

    void setAvailable(bool status);
};

#endif

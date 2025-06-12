#ifndef CAR_H
#define CAR_H

#include "Vehicle.h"

class Car : public Vehicle {
public:
    Car(int id, const QString& brand, const QString& model, double rate, bool available);
};

#endif

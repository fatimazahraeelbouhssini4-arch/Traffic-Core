#ifndef CAR_H
#define CAR_H
#include "Vehicule.h"

class Car : public Vehicule {
public:
    // Vitesse max: 30.0, Accélération: 3.5
    Car(Vector3 pos, Model m);
    void update(float deltaTime) override;
};
#endif
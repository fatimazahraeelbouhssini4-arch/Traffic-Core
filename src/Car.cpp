#include "Car.h"

Car::Car(Vector3 pos, Model m) : Vehicule(pos, 30.0f, 3.5f, m, 1.0f, BLUE) {}

void Car::update(float deltaTime) {
    updateMovement(deltaTime);
}
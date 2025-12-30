#include "Truck.h"

Truck::Truck(Vector3 pos, Model m) : Vehicle(pos, 12.0f, 0.4f, m, 2.0f, RED) {}

void Truck::update(float deltaTime) {
    updateMovement(deltaTime);
}
#include "VehiculeFactory.h"
#include "Car.h"
#include "Bus.h"
#include "Truck.h"
#include "ModelManager.h"

Vehicule* VehiculeFactory::createVehicule(VehiculeType type, Vector3 position) {
    ModelManager& mm = ModelManager::getInstance();

    switch (type) {
        case VehiculeType::CAR:
            return new Car(position, mm.getRandomModel("CAR"));
        case VehiculeType::BUS:
            return new Bus(position, mm.getRandomModel("BUS"));
        case VehiculeType::TRUCK:
            return new Truck(position, mm.getRandomModel("TRUCK"));
        default:
            return nullptr;
    }
}
#include "SimulationManager.h"

SimulationManager& SimulationManager::getInstance() {
    static SimulationManager instance;
    return instance;
}

void SimulationManager::addVehicle(VehicleType type, Vector3 startPos) {
    Vehicle* v = VehicleFactory::createVehicle(type, startPos);
    if (v) {
        // Ici, plus tard, on appellera le Pathfinder pour lui donner un itinéraire
        // std::vector<Vector3> path = pathfinder.calculate(startPos, randomDestination);
        // v->setItinerary(path);
        
        vehicles.push_back(v);
    }
}

void SimulationManager::update(float deltaTime) {
    // 1. Mettre à jour tous les véhicules
    for (auto v : vehicles) {
        v->update(deltaTime);
    }

    // 2. Nettoyer automatiquement les véhicules qui ont fini leur trajet
    clearFinishedVehicles();
}

void SimulationManager::draw() {
    for (auto v : vehicles) {
        v->draw();
    }
}

void SimulationManager::clearFinishedVehicles() {
    for (int i = vehicles.size() - 1; i >= 0; i--) {
        if (vehicles[i]->hasReachedDestination()) {
            delete vehicles[i];
            vehicles.erase(vehicles.begin() + i);
        }
    }
}

void SimulationManager::cleanup() {
    for (auto v : vehicles) {
        delete v;
    }
    vehicles.clear();
}
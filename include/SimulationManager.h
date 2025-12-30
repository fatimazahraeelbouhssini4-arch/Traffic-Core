#ifndef SIMULATION_MANAGER_H
#define SIMULATION_MANAGER_H

#include <vector>
#include "Vehicule.h"
#include "VehiculeFactory.h"
// #include "TrafficGraph.h" // Inclus ton système de routes ici

class SimulationManager {
private:
    std::vector<Vehicule*> vehicules;
    // TrafficGraph cityGraph; // Ton graphe de routes

    SimulationManager() {} // Constructeur privé (Singleton)

public:
    static SimulationManager& getInstance();

    // Gestion des véhicules
    void addVehicule(VehiculeType type, Vector3 startPos);
    void removeVehicule(int index);
    void clearFinishedVehicules(); // Supprime les voitures arrivées à destination

    // Boucle de simulation
    void update(float deltaTime);
    void draw();

    // Nettoyage final
    void cleanup();

    // Getters
    int getVehiculeCount() const { return (int)vehicules.size(); }
    
    // Empêcher la copie
    SimulationManager(const SimulationManager&) = delete;
    void operator=(const SimulationManager&) = delete;
};

#endif
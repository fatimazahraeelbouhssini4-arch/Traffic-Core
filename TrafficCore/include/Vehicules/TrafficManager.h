#ifndef TRAFFICMANAGER_H
#define TRAFFICMANAGER_H

#include "Vehicule.h"
#include <vector>
#include <memory>
#include "../RoadNetwork.h"

class TrafficManager {
private:
    std::vector<std::unique_ptr<Vehicule>> vehicles;
    // Optional pointer to the road network for leader assignment
    RoadNetwork* network = nullptr;

public:
    void addVehicle(std::unique_ptr<Vehicule> vehicle);
    void update(float deltaTime);
    void draw();
    const std::vector<std::unique_ptr<Vehicule>>& getVehicles() const { return vehicles; }
    int getVehicleCount() const { return static_cast<int>(vehicles.size()); }

    void setRoadNetwork(RoadNetwork* net) { network = net; }
};

#endif

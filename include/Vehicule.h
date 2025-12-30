#ifndef VEHICLE_H
#define VEHICLE_H

#include "raylib.h"
#include "raymath.h"
#include <vector>

class Vehicule {
protected:
    // Propriétés physiques et visuelles
    Vector3 position;
    float rotationAngle; // En degrés, pour l'orientation 3D
    float scale;
    Model model;
    Color debugColor;

    // Logique de mouvement et itinéraire
    std::vector<Vector3> itinerary; // Liste de points (intersections) à suivre
    int currentTargetIndex;         // Indice du point vers lequel on va
    
    float currentSpeed;
    float maxSpeed;
    float acceleration;

    // États de la simulation
    bool isFinished; // True si le véhicule a atteint sa destination finale
    bool isWaiting;  // True si le véhicule est stoppé (feu rouge, obstacle)

public:
    Vehicule(Vector3 startPos, float maxSpd, float accel, Model mdl, float sc, Color dColor);
    virtual ~Vehicule() {}

    // Méthodes principales
    virtual void update(float deltaTime) = 0; // Spécifique à chaque type (Car, Bus...)
    virtual void draw();

    // Logique de déplacement commune
    void updateMovement(float dt);
    
    // Gestion de l'itinéraire
    void setItinerary(const std::vector<Vector3>& newPath);
    
    // Accesseurs
    bool hasReachedDestination() const { return isFinished; }
    void setWaiting(bool wait) { isWaiting = wait; }
    Vector3 getPosition() const { return position; }
    float getCurrentSpeed() const { return currentSpeed; }
};

#endif
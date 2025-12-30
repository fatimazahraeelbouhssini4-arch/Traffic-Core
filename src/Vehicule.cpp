#include "Vehicule.h"

Vehicule::Vehicule(Vector3 startPos, float maxSpd, float accel, Model mdl, float sc, Color dColor)
    : position(startPos), maxSpeed(maxSpd), acceleration(accel), model(mdl), 
      scale(sc), debugColor(dColor) 
{
    rotationAngle = 0.0f;
    currentSpeed = 0.0f;
    currentTargetIndex = 0;
    isFinished = false;
    isWaiting = false;
}

void Vehicule::setItinerary(const std::vector<Vector3>& newPath) {
    itinerary = newPath;
    currentTargetIndex = 0;
    isFinished = false;
    if (!itinerary.empty()) {
        position = itinerary[0]; // On place le véhicule au point de départ
    }
}

void Vehicule::updateMovement(float dt) {
    // Si pas de trajet, ou si arrivé, ou si en attente : on ne bouge pas
    if (itinerary.empty() || isFinished) return;

    if (isWaiting) {
        // Freinage d'urgence / Arrêt
        currentSpeed = Lerp(currentSpeed, 0.0f, dt * 5.0f);
        if (currentSpeed < 0.1f) currentSpeed = 0.0f;
    } else {
        // Accélération progressive
        currentSpeed = Lerp(currentSpeed, maxSpeed, dt * acceleration);
    }

    if (currentSpeed <= 0.0f) return;

    // 1. Déterminer la cible actuelle
    Vector3 target = itinerary[currentTargetIndex];
    
    // 2. Calculer la direction et la distance
    Vector3 direction = Vector3Subtract(target, position);
    float distance = Vector3Length(direction);

    // 3. Vérifier si on a atteint le point actuel
    if (distance < 0.5f) {
        currentTargetIndex++;
        if (currentTargetIndex >= itinerary.size()) {
            isFinished = true;
            currentSpeed = 0.0f;
            return;
        }
        return;
    }

    // 4. Rotation : Orienter le modèle vers la cible
    // On utilise atan2 sur les axes X et Z (le plan du sol en 3D)
    float targetAngle = atan2f(direction.x, direction.z) * RAD2DEG;
    // Transition fluide de la rotation (LerpAngle n'existe pas nativement, on simplifie ici)
    rotationAngle = targetAngle;

    // 5. Déplacement physique
    Vector3 velocity = Vector3Scale(Vector3Normalize(direction), currentSpeed * dt);
    position = Vector3Add(position, velocity);
}

void Vehicule::draw() {
    if (model.meshCount == 0) {
        // Dessin d'un cube si le GLB est manquant
        DrawCube(position, 2.0f, 2.0f, 4.0f, debugColor);
        DrawCubeWires(position, 2.1f, 2.1f, 4.1f, BLACK);
    } else {
        // Dessin du modèle 3D orienté
        DrawModelEx(model, position, {0, 1, 0}, rotationAngle, {scale, scale, scale}, WHITE);
    }
}
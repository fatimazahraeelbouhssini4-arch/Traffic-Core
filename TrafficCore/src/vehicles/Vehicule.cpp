#include "Vehicules/Vehicule.h"

Vehicule::Vehicule(Vector3 startPos, float maxSpd, float accel, Model mdl, float sc, Color dColor)
    : position(startPos),
      maxSpeed(maxSpd),
      acceleration(accel),
      model(mdl),
      scale(sc),
      debugColor(dColor)
{
    rotationAngle = 0.0f;
    currentSpeed = 0.0f;
    currentTargetIndex = 0;
    isFinished = false;
    isWaiting = false;
    // traffic defaults
    state = State::DRIVING;
    leader = nullptr;
    desiredSpacing = 4.0f;
    minSpacing = 1.5f;
    timeHeadway = 1.0f;
    // IDM defaults
    idmAccel = 1.5f;
    idmDecel = 2.0f;
    idmDelta = 4.0f;
}

Vehicule::~Vehicule() {
    if (model.meshCount > 0) {
        UnloadModel(model);
    }
}

void Vehicule::setItinerary(const std::vector<Vector3>& newPath) {
    itinerary = newPath;
    currentTargetIndex = 0;
    isFinished = false;
    if (!itinerary.empty()) {
        position = itinerary[0];
    }
}

void Vehicule::updateMovement(float dt) {
    if (itinerary.empty() || isFinished) return;

    // Update state machine and leader following
    updateState(dt);

    if (isWaiting || state == State::STOPPED || state == State::PARKED) {
        currentSpeed = Lerp(currentSpeed, 0.0f, dt * 5.0f);
        if (currentSpeed < 0.1f) currentSpeed = 0.0f;
    } else if (leader != nullptr && state == State::FOLLOWING) {
        followLeader(dt);
    } else {
        currentSpeed = Lerp(currentSpeed, maxSpeed, dt * acceleration);
    }

    if (currentSpeed <= 0.0f) return;

    Vector3 target = itinerary[currentTargetIndex];
    Vector3 direction = Vector3Subtract(target, position);
    float distance = Vector3Length(direction);

    if (distance < 0.5f) {
        currentTargetIndex++;
        if (currentTargetIndex >= itinerary.size()) {
            isFinished = true;
            currentSpeed = 0.0f;
        }
        return;
    }

    float targetAngle = atan2f(direction.x, direction.z) * RAD2DEG;
    rotationAngle = targetAngle;

    Vector3 velocity = Vector3Scale(Vector3Normalize(direction), currentSpeed * dt);
    position = Vector3Add(position, velocity);
}

void Vehicule::updateState(float dt) {
    if (isWaiting) {
        state = State::WAITING_AT_INTERSECTION;
        return;
    }

    if (leader != nullptr) {
        float dist = Vector3Distance(position, leader->getPosition());
        if (dist < desiredSpacing * 1.5f) {
            state = State::FOLLOWING;
            return;
        }
    }

    state = State::DRIVING;
}

void Vehicule::followLeader(float dt) {
    if (leader == nullptr) return;

    float dist = Vector3Distance(position, leader->getPosition());
    float v = currentSpeed;
    float v0 = std::max(0.1f, maxSpeed);
    float s0 = minSpacing;
    float T = timeHeadway;
    float deltaV = v - leader->getCurrentSpeed(); // closing speed (positive if we are faster)

    // safe handle for very small distances
    float s = std::max(0.1f, dist);

    // desired gap s* = s0 + v*T + v*deltaV / (2*sqrt(a*b))
    float sqrtTerm = 2.0f * sqrtf(idmAccel * idmDecel);
    float sStar = s0 + v * T + (v * deltaV) / std::max(0.01f, sqrtTerm);

    // IDM acceleration
    float accel = idmAccel * (1.0f - powf(v / v0, idmDelta) - (sStar / s) * (sStar / s));

    // integrate speed
    currentSpeed += accel * dt;
    if (currentSpeed < 0.0f) currentSpeed = 0.0f;
    if (currentSpeed > maxSpeed) currentSpeed = maxSpeed;
}

void Vehicule::draw() {
    if (model.meshCount == 0) {
        DrawCube(position, 2.0f, 2.0f, 4.0f, debugColor);
        DrawCubeWires(position, 2.1f, 2.1f, 4.1f, BLACK);
    } else {
        DrawModelEx(model, position, {0,1,0}, rotationAngle, {scale,scale,scale}, WHITE);
    }
}

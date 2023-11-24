// Particle.cpp
#include "Particle.h"

Particle::Particle(double x, double y, double mass) : x(x), y(y), mass(mass), forceX(0.0), forceY(0.0) {}

void Particle::updatePosition(double deltaTime) {
    // Update position logic based on velocity (add your own logic here)
    // Here, I'm just adding forces to the current position; you'll need to modify this based on your simulation
    x += forceX * deltaTime;
    y += forceY * deltaTime;
}

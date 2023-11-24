#include "Particle.h"

Particle::Particle(double x, double y, double mass) : x(x), y(y), mass(mass), forceX(0.0), forceY(0.0) {}

void Particle::updatePosition(double deltaTime) {
    // Assuming a simple Euler integration for demonstration purposes
    double damping = 0.99;  // Adjust as needed
    double accelerationX = forceX / mass;
    double accelerationY = forceY / mass;

    // Update velocity
    forceX = 0.0;
    forceY = 0.0;

    // Update position
    x += deltaTime * (damping * accelerationX);
    y += deltaTime * (damping * accelerationY);
}

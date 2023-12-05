#include "Particle.h"
#include <iostream>

constexpr double damping = 0.99; // Add damping as a constant

//Particle::Particle(double x, double y, double mass) : x(x), y(y), mass(mass), forceX(0.0), forceY(0.0), velocityX(0.0), velocityY(0.0) {}

void Particle::updatePosition(double deltaTime) {
    // Verlet integration
    double halfDeltaTime = 0.5 * deltaTime;

    // Update velocity (midpoint method)
    velocityX += (forceX / mass) * halfDeltaTime;
    velocityY += (forceY / mass) * halfDeltaTime;

    // Apply damping to the velocity components
    velocityX *= damping;
    velocityY *= damping;

    // Update position using Verlet integration
    x += velocityX * deltaTime + 0.5 * forceX / mass * deltaTime * deltaTime;
    y += velocityY * deltaTime + 0.5 * forceY / mass * deltaTime * deltaTime;

    // Reset forces for the next iteration
    forceX = 1.0e-10;
    forceY = 1.0e-10;
}



#include "Particle.h"
#include <iostream>

constexpr double damping = 0.99; // Add damping as a constant

//Particle::Particle(double x, double y, double mass) : x(x), y(y), mass(mass), forceX(0.0), forceY(0.0), velocityX(0.0), velocityY(0.0) {}

void Particle::updatePosition(double deltaTime) {
    x += velocityX * deltaTime;
    y += velocityY * deltaTime;
}






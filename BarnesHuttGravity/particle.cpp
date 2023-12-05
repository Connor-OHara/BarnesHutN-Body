#include "Particle.h"
#include <iostream>

constexpr double damping = 0.99; // Add damping as a constant

//Particle::Particle(double x, double y, double mass) : x(x), y(y), mass(mass), forceX(0.0), forceY(0.0), velocityX(0.0), velocityY(0.0) {}

void Particle::updatePosition(double deltaTime) {

    // Verlet integration
    double halfDeltaTime = 0.5 * deltaTime;

    // Debugging output
    //std::cout << "Particle before update - X: " << x << ", Y: " << y << std::endl;
    //std::cout << "Forces before update - X: " << forceX << ", Y: " << forceY << std::endl;

    // Update velocity (midpoint method)
    velocityX += (forceX / mass) * halfDeltaTime;
    velocityY += (forceY / mass) * halfDeltaTime;

    // Update position
    x += velocityX * deltaTime;
    y += velocityY * deltaTime;


    // Update velocity (midpoint method)
    velocityX += (forceX / mass) * halfDeltaTime;
    velocityY += (forceY / mass) * halfDeltaTime;

    // Debugging output
    //std::cout << "Forces after update - X: " << forceX << ", Y: " << forceY << std::endl;
    //std::cout << "Particle after update - X: " << x << ", Y: " << y << std::endl;

    // Reset forces for the next iteration
    forceX = 0.0;
    forceY = 0.0;
}




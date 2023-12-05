#include "Particle.h"
#include <iostream>

constexpr double damping = 0.99; // Add damping as a constant

//Particle::Particle(double x, double y, double mass) : x(x), y(y), mass(mass), forceX(0.0), forceY(0.0), velocityX(0.0), velocityY(0.0) {}

void Particle::updatePosition(double deltaTime) {
    double accelerationX = forceX / mass;
    double accelerationY = forceY / mass;

    // Debug output for forces, acceleration, velocity, and position before the update
    //std::cout << "Forces - X: " << forceX << ", Y: " << forceY << std::endl;
    //std::cout << "Initial Velocity - X: " << velocityX << ", Y: " << velocityY << std::endl;

    // Update velocity with damping
    velocityX += deltaTime * (damping * accelerationX);
    velocityY += deltaTime * (damping * accelerationY);

    // Debug output for the updated acceleration, velocity, and position
    //std::cout << "Acceleration - X: " << accelerationX << ", Y: " << accelerationY << std::endl;
    //std::cout << "Post Update Velocity - X: " << velocityX << ", Y: " << velocityY << std::endl;
    //std::cout << "PART Initial Position - X: " << x << ", Y: " << y << std::endl;


    // Update position based on the updated velocity
    x += deltaTime * velocityX;
    y += deltaTime * velocityY;

    // Debug output for the updated velocity and position
    //std::cout << "Post Update Velocity - X: " << velocityX << ", Y: " << velocityY << std::endl;
    //std::cout << "PART Updated Position - X: " << x << ", Y: " << y << std::endl;

    // Reset forces after updating position
    forceX = 0.0;
    forceY = 0.0;
}



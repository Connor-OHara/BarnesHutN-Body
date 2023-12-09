/*
Author: Connor O'Hara
Class: ECE4122
Last Date Modified: 12/8/23
Description:
A particle, which is a star planet etc. Exists as a point source
*/

#include "Particle.h"
#include <iostream>

constexpr double damping = 0.99; // Add damping as a constant

//Particle::Particle(double x, double y, double mass) : x(x), y(y), mass(mass), forceX(0.0), forceY(0.0), velocityX(0.0), velocityY(0.0) {}

//update our particle over some given change in time dT
void Particle::updatePosition(double deltaTime)
{
    //std::cout << "Particle Position Before Update - X: " << x << ", Y: " << y << std::endl;
    //std::cout << "Forces Before Update - ForceX: " << forceX << ", ForceY: " << forceY << std::endl;

    // Update velocity using forces
    velocityX += (forceX / mass) * deltaTime;
    velocityY += (forceY / mass) * deltaTime;

    // Apply damping to the velocity components
    velocityX *= damping;
    velocityY *= damping;

    // Update position using Euler integration
    x += velocityX * deltaTime;
    y += velocityY * deltaTime;

    // Reset forces for the next iteration
    forceX = 0.0;
    forceY = 0.0;

    //std::cout << "Particle Position After Update - X: " << x << ", Y: " << y << std::endl;
    //std::cout << "Forces After Update - ForceX: " << forceX << ", ForceY: " << forceY << std::endl;
}




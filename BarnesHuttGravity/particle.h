/*
Author: Connor O'Hara
Class: ECE4122
Last Date Modified: 12/8/23
Description:
Header file for the Particle, which represents a star, planet or other object in our system
*/

#pragma once

#include <cmath>
#include <iostream>

//Define our particle, and give it a shell for its standard functions and attributes
class Particle {
public:
    double x;
    double y;
    double mass;
    double forceX;
    double forceY;
    double velocityX, velocityY;

    //Particle(double x, double y, double mass);

    void updatePosition(double deltaTime);

    Particle(double x, double y, double _mass) : x(x), y(y), mass(_mass), forceX(1.0), forceY(1.0), velocityX(1.0), velocityY(1.0) {}




    // Ensure that the distance is never zero to avoid division by zero
    double getDistance(const Particle& other) const 
    {
        double dx = other.x - x;
        double dy = other.y - y;
        return std::max(std::sqrt(dx * dx + dy * dy), 1e-6);
    }

};

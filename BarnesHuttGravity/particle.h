#pragma once

#include <cmath>
#include <iostream>


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

    Particle(double x, double y, double mass) : x(x), y(y), mass(mass), forceX(0.0), forceY(0.0), velocityX(0.0), velocityY(0.0) {
        // Set initial forces to small non-zero values
        //forceX = 1.0e-10;
        //forceY = 1.0e-10;
    }



    // Ensure that the distance is never zero to avoid division by zero
    double getDistance(const Particle& other) const {
        double dx = other.x - x;
        double dy = other.y - y;
        return std::max(std::sqrt(dx * dx + dy * dy), 1e-6);
    }

};

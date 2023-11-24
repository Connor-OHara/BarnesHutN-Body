// Particle.h
#pragma once

class Particle {
public:
    double x, y;
    double mass;
    double forceX, forceY; // Added forces for the particle

    Particle(double x, double y, double mass);

    // Update position based on velocity
    void updatePosition(double deltaTime);
};

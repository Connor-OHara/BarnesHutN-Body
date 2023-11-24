#pragma once

class Particle {
public:
    double x;
    double y;
    double mass;
    double forceX;
    double forceY;

    Particle(double x, double y, double mass);

    void updatePosition(double deltaTime);
};

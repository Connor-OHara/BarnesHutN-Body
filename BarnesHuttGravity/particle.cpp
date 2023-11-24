#include "particle.h"
#include "Vect.h"


//Getters and setters

void Particle::setLocation(double x_, double y_) {
    this->x = x_;
    this->y = y_;
}

void Particle::setLocation(const Vect& force) {
    // Assuming you have getX and getY methods
    double newX = getX() + force.x;
    double newY = getY() + force.y;

    // Assuming you have setX and setY methods
    setX(newX);
    setY(newY);
}

double Particle::getX() const {
    return this->x;
}

double Particle::getY() const {
    return this->y;
}

void Particle::setX(double newX) {
    this->x = newX;
}

void Particle::setY(double newY) {
    this->y = newY;
}

void Particle::setMass(double mass_) {
    this->mass = mass_;
}

double Particle::getMass() const {
    return this->mass;
}

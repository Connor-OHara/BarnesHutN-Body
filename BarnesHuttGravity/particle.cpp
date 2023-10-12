#include "particle.h"

Particle::Particle(double x_, double y_, double mass)
{
	this->x = x_;
	this->y = y_;
	this->mass = mass;
}

Particle::Particle(double x_, double y_, double mass, double accX, double accY)
{
	this->x = x_;
	this->y = y_;
	this->mass = mass;
	this->accelerationX = accX;
	this->accelerationY = accY;
}


//Getters and setters

void Particle::setLocation(double x_, double y_)
{
	this->x = x_;
	this->y = y_;
}


double Particle::getX() const
{
	return this->x;
}

double Particle::getY() const
{
	return this->y;
}

void Particle::setMass(double mass_)
{
	this->mass = mass;
}

double Particle::getMass() const
{
	return this->mass;
}



void Particle::setAccX(double acc)
{
	this->accelerationX = acc;
}

void Particle::setAccY(double acc)
{
	this->accelerationY = acc;
}

double Particle::getAccX()
{
	return this->accelerationX;
}

double Particle::getAccY()
{
	return this->accelerationY;
}

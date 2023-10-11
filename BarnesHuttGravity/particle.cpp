#include "particle.h"

Particle::Particle(double x_, double y_, double z_, double mass)
{
	this->x = x_;
	this->y = y_;
	this->mass = mass;
}

Particle::Particle(double x_, double y_, double mass, double acc)
{
	this->x = x_;
	this->y = y_;
	this->mass = mass;
	this->acceleration = acc;
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



void Particle::setAcc(double acc)
{
	this->acceleration = acc;
}

double Particle::getAcc()
{
	return this->acceleration;
}

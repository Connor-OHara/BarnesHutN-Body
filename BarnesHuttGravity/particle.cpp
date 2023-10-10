#include "particle.h"

void Particle::setLocation(double x_, double y_, double z_)
{
}

void Particle::setMass(double mass_)
{
	this->mass = mass;
}

double Particle::getMass() const
{
	return this->mass;
}

double Particle::getX() const
{
	return this->x;
}

double Particle::getY() const
{
	return this->y;
}

double Particle::getZ() const
{
	return this->z;
}

void Particle::setAcc(double acc)
{
	this->acceleration = acc;
}

double Particle::getAcc()
{
	return this->acceleration;
}

#pragma once
/*
Author: Connor O'Hara
Class : ECE4122
Last Date Modified : 10/10/2023
Description :
This is the file that defines a Particle. 
*/

#include <vector>

using namespace std;

#ifndef N_PART
#define N_PART

/*
* Description:
* Header class describing important member functions, constructor, and descriptors for the class.
* The Point Charge Class describes a singular point charge in 3D space, giving it attributes about its location and charge quantity.
*/
class Particle
{
public:

	//Default Constructors with overloads.
	Particle(double x_, double y_, double mass);
	Particle(double x_, double y_, double mass, double acc);
	Particle() : x(0.0), y(0.0), mass(0.0), acceleration(0.0) {};

	// Sets the default location 
	void setLocation(double x_, double y_);
	
	
	//getter and setter for mass
	void setMass(double mass_);
	double getMass() const;

	double getX() const;
	double getY() const;


	void setAcc(double acc);
	double getAcc();

protected:
	/// <summary>
	///	Protected attributes.
	/// </summary>
	double x, y, mass = 1.0, acceleration = 0.0;
};

#endif 
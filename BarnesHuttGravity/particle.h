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
	Particle(double x_, double y_, double z_, double mass);
	Particle(double x_, double y_, double z_, double mass, double acc);
	Particle() : x(0.0), y(0.0), z(0.0), mass(0.0) {};

	// Sets the default location 
	void setLocation(double x_, double y_, double z_);
	//returns a pointer to an array where arr[0] == x, arr[1] == y, arr[2] == z. Discrete indice for O(1)
	std::vector<double> getLocation() const;

	//getter and setter for mass
	void setMass(double mass_);
	double getMass() const;

	double getX() const;
	double getY() const;
	double getZ() const;


	void setAcc(double acc);
	double getAcc();

protected:
	/// <summary>
	///	Protected attributes.
	/// </summary>
	double x, y, z, mass, acceleration;
};

#endif 
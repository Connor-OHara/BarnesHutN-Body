#pragma once


#include <vector>
#include "particle.h"
#include "Vect.h"
#include <memory>

#ifndef N_NODE
#define N_NODE
/*
* This class Defines the nodes in the Barnes Hut Quadtree
* The nodes potentially contain subnodes, which potentially contain the particles themselves.
* Functions include creation of the subnodes, insertion of the particles into those subnodes,
* and destruction of the tree of subnodes.
* 
*/
class Node
{
public:

	//Constructor
	Node(const Vect& UL, const Vect& LR, Node* _parent);

	void Destroy();

	std::vector<std::shared_ptr<Particle>> Particles;

	//Each node will have at most 4 childen, one for each quadrant.
	//If upraded to Octree... you can see how it grows
	//Node pointers for our subnodes
	Node* _node[4];
	//std::vector<Node*> childNode;

	bool hasChild;

	double mass;
	unsigned int particleCount = 0;




	//enumerate our quadrants, 0 indexed
	enum Quadrant
	{
		NE = 0,
		NW,
		SW,
		SE,
		undefined
	};



	Vect getCent() const;
	Vect getUL() const;
	Vect getLR() const;
	Vect getCMass() const;
	float getTheta();
	bool rootCheck() const;

	void setCent(Vect cent);
	void setUL(Vect ul);
	void setLR(Vect lr);
	void setCMass(Vect cm);
	void setTheta(float _theta);



	//Build our tree
	void Add(std::shared_ptr<Particle> newParticle);
	//Compute our Center of Node Mass
	void ComputeMassDistribution();
	//Get quadrant helper for Add(Particle particle)
	Node::Quadrant getQuadrant(double x, double y) const;

	//Compute Force and Acceleration for particles in node
	Vect CalcForce(Particle part);
	//Vect calAccel(Particle part1, Particle part2);

	//Create a new subnode
	Node* CreateSubNode(Quadrant quad);

	static Node* root;

	//Gravitational Constant
	static constexpr double G = 6.67428e-11;


private:
	/*
	* 2D Vector representation of our top left, bottom right, center, and center of mass 
	* of our Node. This helps us bound our box
	*/
	Vect centerMass, UL, LR, Cent;
	Node* parent;
	float theta;
};
#endif
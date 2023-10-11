#pragma once


#include <vector>
#include "particle.cpp"

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

	//Constructors
	Node();
	Node(unsigned int depth);

	void Destroy();

	std::vector<Particle*> Particles;
	std::vector<Node*> childNode;

	bool parent;

	float X, Y, mass, centerMassX, centerMassY;
	unsigned int depth;

	float cellWidthX, cellWidthY;

	//Build our tree
	void Add();
	//Compute our Center of Node Mass
	void ComputeMassDistribution();



};
#endif
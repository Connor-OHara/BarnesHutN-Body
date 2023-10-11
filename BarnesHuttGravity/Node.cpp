#include "Node.h"

//psuedocoderef: https://beltoforion.de/en/barnes-hut-galaxy-simulator/



Node::Node()
{
    parent = false;
    depth = 0;
}

Node::Node(unsigned int currDepth)
{
    parent = false;
    depth = currDepth;
}

void Node::Destroy()
{
	Particles.clear();
	parent = false;

	for (unsigned int i = 0; i < childNode.size(); i++)
	{
		childNode[i]->Destroy();
		delete childNode[i];
	}

	childNode.clear();

}

void Node::Add()
{
    /*
    * if number of particles in this node > 1
  {
    quad = GetQuadrant(newParticle);
    
    if subnode(quad) does not exist
      create subnode(quad)

    subnode(quad)->InsertToNode(newParticle)
  }
  else if number of particles in this node == 1
  {
    quad = GetQuadrant(existingParticle);
    if subnode(quad) does not exist
      create subnode(quad)
    subnode(quad)->InsertToNode(existingParticle)

    quad = GetQuadrant(newParticle);
    if subnode(quad) does not exist
      create subnode(quad)
    subnode(quad)->InsertToNode(newParticle);
  }
  else if node is empty
  {
    store newParticle in node as existingParticle
  }

  Increase number of particles
    */


}

void Node::ComputeMassDistribution()
{
	/*
	* if number of particles in this node equals 1
  {
    CenterOfMass = Particle.Position
    Mass = Particle.Mass  
  }
  else
  {
    // Compute the center of mass based on the masses of 
    // all child quadrants and the center of mass as the 
    // center of mass of the child quadrants weightes with 
    // their mass
    for all child quadrants that have particles in them
    {
      Quadrant.ComputeMassDistribution
      Mass += Quadrant.Mass
      CenterOfMass +=  Quadrant.Mass * Quadrant.CenterOfMass
    }
    CenterOfMass /= Mass
  }
	* 
	*/











}

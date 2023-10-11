#include "Node.h"
#include <stdexcept>
#include <stdio.h>
#include <iostream>
#include "Vect.h"


//psuedocoderef: https://beltoforion.de/en/barnes-hut-galaxy-simulator/



Node::Node()
{
    hasChild = false;
    particleCount = 1;
}

Node::Node(const Vect &_UL, const Vect &_LR, Node* parent)
{
    hasChild = false;
    mass = 0;
    centerMass;
    UL = _UL;
    LR = _LR;
    Cent = (_UL.x + (_LR.x - _UL.x) / 2.0, _UL.y + (_LR.y - _UL.y) / 2.0);
    particleCount = 1;
}


Vect Node::getCent() const
{
    return this->Cent;
}

Vect Node::getUL() const
{
    return this->UL;
}

Vect Node::getLR() const
{
    return this->LR;
}

Vect Node::getCMass() const
{
    return this->centerMass;
}

void Node::setCent(Vect cent)
{
    this->Cent = cent;
}

void Node::setUL(Vect ul)
{
    this->UL = ul;
}

void Node::setLR(Vect lr)
{
    this->LR = lr;
}

void Node::setCMass(Vect cm)
{
    this->centerMass = cm;
}



//Returns whether leaf is empty
bool Node::leafCheck()
{
    return  this->_node[0] == nullptr &&
            this->_node[1] == nullptr &&
            this->_node[2] == nullptr &&
            this->_node[3] == nullptr;
}




Node::Quadrant Node::getQuadrant(double x, double y) const
{
    if (x >= this->Cent.x && y >= this->Cent.y)
    {
        return NE;
    }
    else if (x <= this->Cent.x && y >= this->Cent.y)
    {
        return NW;
    }
    else if (x <= this->Cent.y && y <= this->Cent.y)
    {
        return SW;
    }
    else if (x >= this->Cent.x && y <= this->Cent.y)
    {
        return SE;
    }
    else
    {
        throw std::runtime_error("Wonky Quadrant determination. Not cool bro.");
    }
}




/*Create our Subnode. More than one object in tree, so we try and divide up the tree
* to be smaller. This allows us that fast access time that we love
*/
Node* Node::CreateSubNode(Quadrant quad)
{
    //Switch creates and returns new Node. Sets the quadrant based off of where the target particle exists
    switch (quad)
    {
    case NE:
        return new Node(this->Cent, this->LR, this);
    case NW:
        return new Node(Vect(this->UL.x, this->Cent.y), Vect(this->Cent.x, this->LR.y), this);
    case SW:
        return new Node(this->UL, this->Cent, this);
    case SE:
        return new Node(Vect(this->Cent.x, this->LR.y), Vect(this->UL.x, this->Cent.y), this);
    default:
        throw std::runtime_error("Cannot Create New Subnode. Quadrant is not set");
    }
}





void Node::Add(Particle newParticle)
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


    if (this->particleCount > 1)
    {
        Node::Quadrant quad = getQuadrant(newParticle.getX(), newParticle.getY());

        //Create a subnode
        if (!this->_node[quad])
        {
            _node[quad] = CreateSubNode(quad);
        }

        _node[quad]->Add(newParticle);

    }
    else if (this->particleCount == 1)
    {
        int quad = getQuadrant(this->Particles.at(0)->getX(), this->Particles.at(0)->getY());
    }
    else
    {
        this->Particles.push_back(&newParticle);
    }


    this->particleCount += 1;

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



void Node::Destroy()
{
    Particles.clear();
    hasChild = false;

    for (unsigned int i = 0; i < 4; i++)
    {
        //What do I do, Lord?
        //Destroy the child
        _node[i]->Destroy();
        //Corrupt them all
        delete _node[i];
    }

}
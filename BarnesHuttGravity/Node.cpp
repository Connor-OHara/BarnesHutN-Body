#include "Node.h"
#include <stdexcept>
#include <stdio.h>
#include <iostream>
#include "Vect.h"
#include <omp.h>



Node::Node(const Vect &_UL, const Vect &_LR, Node* _parent)
{
    hasChild = false;
    mass = 0;
    centerMass;
    UL = _UL;
    LR = _LR;
    Cent = (_UL.x + (_LR.x - _UL.x) / 2.0, _UL.y + (_LR.y - _UL.y) / 2.0);
    particleCount = 1;
    parent = parent;
    theta = 1;
    _node[0] = _node[1] = _node[2] = _node[3] = nullptr;
}

//Getters and setters
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

float Node::getTheta()
{
    return this->theta;
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

void Node::setTheta(float _theta)
{
    this->theta = _theta;
}

bool Node::rootCheck() const
{
    return this->parent == nullptr;
}


// Find what sub-quadrant our target particle will be placed in relative to the Node



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
        //return undefined;
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
        this->hasChild = true;

    }
    else if (this->particleCount == 1)
    {

        if ((this->Particles.at(0)->getX() == newParticle.getX()) && (this->Particles.at(0)->getY() == newParticle.getY()))
        {
            //Do not place at same point
            return;
        }

        //Find the quadrant of already existing particle
        Quadrant quad = getQuadrant(this->Particles.at(0)->getX(), this->Particles.at(0)->getY());

        //We now subdivide our Node. 
        //Deal with old particle
        if (this->_node[quad] == nullptr)
        {
            this->_node[quad] = CreateSubNode(quad);
        }
        _node[quad]->Add(*this->Particles.at(0));


        //insert our new particle
        if (this->_node[quad] == nullptr)
        {
            this->_node[quad] = CreateSubNode(quad);
        }
        _node[quad]->Add(newParticle);



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

    /*
    * 2)  For each subsquare in the quadtree, 
       compute the center of mass and total mass 
       for all the particles it contains.
    */


    //You can almost compare this to the balancing algorithm  used by AVL's



    //* if number of particles in this node equals 1
    if (this->particleCount == 1)
    {
        //Set mass of Node to be that of just our singular particle
        this->mass = this->Particles.at(0)->getMass();
        this->centerMass.x = this->Particles.at(0)->getX();
        this->centerMass.y = this->Particles.at(0)->getY();
    }
    else
    {
        // More than one particle exists! Need to rebalance trees and  recompute
        // CM
        // Compute the center of mass based on the masses of 
        // all child quadrants and the center of mass as the 
        // center of mass of the child quadrants weights with 
        // their mass

        //Find mass of all child masses

        //zero out mass. We will recalculate
        this->mass = 0;
        this->centerMass.x = 0;
        this->centerMass.y = 0;


#pragma omp parallel for 
        for (int i = 0; i < 4; ++i)
        {
            if (this->_node[i])
            {
                //Recalculate for all subnodes
                this->_node[i]->ComputeMassDistribution();
                //compute center of mass and dotal mass
                this->mass += this->_node[i]->mass;
                this->centerMass.x += this->_node[i]->centerMass.x * this->_node[i]->mass;
                this->centerMass.y += this->_node[i]->centerMass.y * this->_node[i]->mass;
            }
        }

        //update our new center of mass
        this->centerMass.x /= this->mass;
        this->centerMass.y /= this->mass;

    }

}











void Node::Destroy()
{
    

    for (unsigned int i = 0; i < 4; i++)
    {
        //What do I do, Lord?
        //Destroy the child
        _node[i]->Destroy();
        //Corrupt them all
        delete _node[i];
    }


    Particles.clear();
    hasChild = false;
    this->particleCount = 0;
    this->mass = 0;
    this->centerMass = Vect(0.0, 0.0);
    

    //Reset bounding box


}
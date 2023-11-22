#include "Node.h"
#include <stdexcept>
#include <stdio.h>
#include <iostream>
#include "Vect.h"
#include <omp.h>
#include <cmath>
#include <math.h>
#include <stack>
#define _USE_MATH_DEFINES


static const double minBoxSize = 0.1;  // Adjust this value as needed


Node* Node::root = nullptr;

Node::Node(const Vect &_UL, const Vect &_LR, Node* _parent) : UL(_UL), LR(_LR), parent(_parent), theta(1.0) 
{
    hasChild = false;
    mass = 0;
    centerMass = Vect(0.0, 0.0);
    Cent.x = (_UL.x + (_LR.x - _UL.x) / 2.0);
    Cent.y = (_UL.y + (_LR.y - _UL.y) / 2.0);
    particleCount = 0;
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
    else if (x <= this->Cent.x && y <= this->Cent.y)
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
        throw std::runtime_error("Wonky Quadrant determination during GetQuadrant(x,y) call. ");
    }
}




/*Create our Subnode. More than one object in tree, so we try and divide up the tree
* to be smaller. This allows us that fast access time that we love
*/
Node* Node::CreateSubNode(Quadrant quad)

{


    // Check bounding box size before creating a new subnode
    double boxSize = std::max((LR.x - UL.x), (LR.y - UL.y));
    if (boxSize < minBoxSize) {
        return nullptr; // Do not create further subnodes
    }


    //Switch creates and returns a new Node. Sets the quadrant based on where the target particle exists
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
void Node::Add(std::shared_ptr<Particle> newParticle)
{
    std::stack<Node*> stack;
    stack.push(this);
    
    /*
    if (this->particleCount > 0)
    {
        for (const auto& existingParticle : Particles)
        {
            double dx = newParticle.getX() - existingParticle->getX();
            double dy = newParticle.getY() - existingParticle->getY();
            double distance = std::sqrt(dx * dx + dy * dy);

            if (distance < minSeparation)
            {
                // Handle the collision by displacing the new particle
                double angle = std::rand() / static_cast<double>(RAND_MAX) * 2.0 * 3.14159265358979323846;
                double displacement = minSeparation - distance;
                double displacementX = std::cos(angle) * displacement;
                double displacementY = std::sin(angle) * displacement;

                newParticle.setLocation(newParticle.getX() + displacementX, newParticle.getY() + displacementY);
            }
        }
    }
    */

    while (!stack.empty()) {
        Node* currentNode = stack.top();
        stack.pop();


        if (currentNode == nullptr) {
            // Handle the case where currentNode is null
            continue;
        }


        if (currentNode->particleCount == 0) {
            currentNode->Particles.push_back(newParticle);
            currentNode->mass = newParticle->getMass();
            currentNode->particleCount = 1;


            // Print ROOT AT only when a new root node is created
            //std::cout << "ROOT AT X:" << Particles.at(0)->getX() << " Y:" << Particles.at(0)->getY() << std::endl;

        }
        else if (currentNode->particleCount == 1) {
            if (*Particles.at(0) == *newParticle) {
                std::cout << "Point Collision During Add At X:" << newParticle->getX() << " Y:" << newParticle->getY() << std::endl;
                return;
            }

            // Determine the quadrant for the existing particle (this->Particles.at(0)):
            Quadrant quad = currentNode->getQuadrant(currentNode->Particles.at(0)->getX(), currentNode->Particles.at(0)->getY());
            // Check if the corresponding subnode for the quadrant exists:
            if (currentNode->_node[quad] == nullptr) {
                // If the subnode doesn't exist, create a new subnode for the quadrant:
                currentNode->_node[quad] = currentNode->CreateSubNode(quad);
            }
            // Recursively add the existing particle to the appropriate subnode:
            currentNode->_node[quad]->Add(currentNode->Particles.at(0));

            // Determine the quadrant for the new particle (newParticle):
            quad = currentNode->getQuadrant(newParticle->getX(), newParticle->getY());
            // Check if the corresponding subnode for the quadrant exists:
            if (currentNode->_node[quad] == nullptr) {
                // If the subnode doesn't exist, create a new subnode for the quadrant:
                currentNode->_node[quad] = currentNode->CreateSubNode(quad);
            }
            // Recursively add the new particle to the appropriate subnode:
            currentNode->_node[quad]->Add(newParticle);


            currentNode->particleCount = 2; // Update particle count after adding both particles

            // Print ROOT AT only when a new root node is created
            //std::cout << "ROOT AT X:" << Particles.at(0)->getX() << " Y:" << Particles.at(0)->getY() << std::endl;
        }
        else if (particleCount > 1) {
            Quadrant quad = currentNode->getQuadrant(newParticle->getX(), newParticle->getY());
            if (currentNode->_node[quad] == nullptr) {
                currentNode->_node[quad] = currentNode->CreateSubNode(quad);
            }
            currentNode->_node[quad]->Add(newParticle);
            currentNode->particleCount += 1;
        }

        if (currentNode->parent) {
            currentNode->parent->mass += newParticle->getMass();
            // Also update the center of mass for the parent
            currentNode->parent->centerMass.x += newParticle->getX() * newParticle->getMass();
            currentNode->parent->centerMass.y += newParticle->getY() * newParticle->getMass();
        }
    }
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

    std::stack<Node*> stack;
    stack.push(this);

    while (!stack.empty()) {
        Node* currentNode = stack.top();
        stack.pop();

        if (currentNode->particleCount == 1)
        {
            currentNode->mass = currentNode->Particles.at(0)->getMass();
            currentNode->centerMass.x = currentNode->Particles.at(0)->getX();
            currentNode->centerMass.y = currentNode->Particles.at(0)->getY();
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
            currentNode->mass = 0;
            currentNode->centerMass.x = 0;
            currentNode->centerMass.y = 0;

#pragma omp parallel for reduction(+:currentNode->mass, currentNode->centerMass.x, currentNode->centerMass.y)
            for (int i = 0; i < 4; ++i)
            {
                if (currentNode->_node[i])
                {
                    //Recalculate for all subnodes
                    currentNode->_node[i]->ComputeMassDistribution();
                    //compute center of mass and dotal mass
                    currentNode->mass += currentNode->_node[i]->mass;
                    currentNode->centerMass.x += currentNode->_node[i]->centerMass.x * currentNode->_node[i]->mass;
                    currentNode->centerMass.y += currentNode->_node[i]->centerMass.y * currentNode->_node[i]->mass;
                }
            }

            //update our new center of mass
            currentNode->centerMass.x /= currentNode->mass;
            currentNode->centerMass.y /= currentNode->mass;



            // Update the center of mass for the parent
            if (currentNode->parent) {
                currentNode->parent->centerMass.x += currentNode->centerMass.x * currentNode->mass;
                currentNode->parent->centerMass.y += currentNode->centerMass.y * currentNode->mass;
            }

        }
    }
}













Vect Node::CalcForce(Particle _part)
{
    //psuedocode from: https://people.eecs.berkeley.edu/~demmel/cs267/lecture26/lecture26.html#link_4
    /*Here is the algorithm for step 3 of Barnes-Hut:

      ... For each particle, traverse the tree 
      ... to compute the force on it.
      For i = 1 to n
          f(i) = TreeForce(i,root)   
      end for

      function f = TreeForce(i,n)
          ... Compute gravitational force on particle i 
          ... due to all particles in the box at n
          f = 0
          if n contains one particle
              f = force computed using formula (*) above
          else 
              r = distance from particle i to 
                     center of mass of particles in n
              D = size of box n
              if D/r < theta
                  compute f using formula (*) above
              else
                  for all children c of n
                      f = f + TreeForce(i,c)
                  end for
              end if
          end if
    */

    //return acceleration vector
    Vect retForce;

    if (this->particleCount == 1)
    {
        //retForce = calAccel(_part, *this->Particles.at(0));
        double radius = sqrt((_part.getX() - this->Particles.at(0)->getX()) * (_part.getX() - this->Particles.at(0)->getX()) + 
            (_part.getY() - this->Particles.at(0)->getY()) * (_part.getY() - this->Particles.at(0)->getY()));
        //compute f using formula
        double force = G * this->mass / (radius * radius * radius);

        //this->Particles.at(0)->setAccX(force * (this->centerMass.x - _part.getX()));
        //this->Particles.at(0)->setAccY(force * (this->centerMass.y - _part.getY()));
        retForce.x = force * (this->centerMass.x - _part.getX());
        retForce.y = force * (this->centerMass.y - _part.getY());



    }
    else
    {
        //r = distance from particle i to center of mass of particles in n
        double radius = sqrt((_part.getX() - this->centerMass.x) * (_part.getX() - this->centerMass.x) + (_part.getY() - this->centerMass.y) * (_part.getY() - this->centerMass.y));

        //D = size of box n
        double D = this->UL.x - this->LR.x;

        //if D/r < theta (our balancing factor)
        //Are we too close to each other for our approximation?
        if (D / radius <= this->theta)
        {
            //compute f using formula
            double force = G * this->mass / (radius * radius * radius);

            //this->Particles.at(0)->setAccX(force * (this->centerMass.x - _part.getX()));
            //this->Particles.at(0)->setAccY(force * (this->centerMass.y - _part.getY()));
            retForce.x = force * (this->centerMass.x - _part.getX());
            retForce.y = force * (this->centerMass.y - _part.getY());

        }
        else
        {
            Vect returnBuffer;
            //for all children c of n
#pragma omp parallel for reduction(+:retForce.x, retForce.y)
            for (int i = 0; i < 4; ++i) 
            {
                //f = f + TreeForce(i,c)
                returnBuffer = _node[i]->CalcForce(_part);

                retForce.x += returnBuffer.x;
                retForce.y += returnBuffer.y;
                

            }
        }

    }

    return retForce;
}








void Node::Destroy()
{
    for (unsigned int i = 0; i < 4; i++)
    {
        if (_node[i] != nullptr)
        {
            // Destroy the child
            _node[i]->Destroy();
            // Delete the child node
            delete _node[i];
            // Set the pointer to nullptr
            _node[i] = nullptr;
        }
    }

    // Reset metadata
    Particles.clear();
    hasChild = false;
    this->particleCount = 0;
    this->mass = 0;
    this->centerMass = Vect(0.0, 0.0);
}


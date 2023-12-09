/*
Author: Connor O'Hara
Class: ECE4122
Last Date Modified: 12/8/23
Description:
The Quadtree, which contains the entirety of the simulation backend. This is instantiated once and contains all of our nodes
*/


#include "Quadtree.h"
#include "Node.h"
#include "Particle.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>
#include <omp.h>


Quadtree::Quadtree(double x, double y, double width, double height, double theta) : root(new Node(x, y, width, height)), theta(theta) {}

Quadtree::~Quadtree() {}

double forceScale = 100.0;  // Adjust this scaling factor as needed - helps with super low movement. meant to help with SFML


//Update the mass of the node and all its subnodes
void Quadtree::updateMass(Node* node) {
    if (node->isLeaf) 
    {
        if (node->particle != nullptr) 
        {
            node->mass = node->particle->mass;
        }
        else 
        {
            node->mass = 0.0;
        }
    }
    else {
        node->mass = 0.0;
        for (int i = 0; i < 4; ++i) 
        {
            if (node->children[i] != nullptr) 
            {
                updateMass(node->children[i].get());
                node->mass += node->children[i]->mass;
            }
        }
    }
}


//Insert a particle into the correct place to build the tree. Will recursively find the correct 
//Node to insert. the top level call has the root node of the tree be the source.
void Quadtree::insert(Node* node, Particle* particle) {
    // Existing particle at this node
    if (node->isLeaf && node->particle != nullptr) 
    {
        // Split the node only if the particle and the existing one are at different positions
        if (node->particle->x != particle->x || node->particle->y != particle->y) 
        {
            node->isLeaf = false;
            split(node, particle->x, particle->y, node->width, node->height);
            moveParticleToChild(node, node->particle);
        }
        // Move the new particle to the appropriate child node
        moveParticleToChild(node, particle);
    }
    // No existing particle, insert the new one
    else if (node->isLeaf && node->particle == nullptr) 
    {
        node->particle = particle;
    }
    // Internal node, move the new particle to the appropriate child node
    else {
        int quadrant = getQuadrant(node, particle->x, particle->y);

        if (node->children[quadrant] == nullptr) 
        {
            double subX = node->x + (quadrant % 2) * node->width / 2;
            double subY = node->y + (quadrant / 2) * node->height / 2;
            node->children[quadrant] = std::make_unique<Node>(subX + node->width / 4, subY + node->height / 4, node->width / 2, node->height / 2);
        }

        insert(node->children[quadrant].get(), particle);
    }

    updateMass(node);
}


// Helper function to move a particle to the appropriate child node
void Quadtree::moveParticleToChild(Node* node, Particle* particle) {
    int quadrant = getQuadrant(node, particle->x, particle->y);

    if (node->children[quadrant] == nullptr) 
    {
        double subX = node->x + (quadrant % 2) * node->width / 2;
        double subY = node->y + (quadrant / 2) * node->height / 2;
        node->children[quadrant] = std::make_unique<Node>(subX + node->width / 4, subY + node->height / 4, node->width / 2, node->height / 2);
    }

    insert(node->children[quadrant].get(), particle);
}


//Divide our Node so that it has its subnodes, this is so that the node does not become saturated, and moves its kids to the subnode
void Quadtree::split(Node* node, double x, double y, double width, double height) {
    double subWidth = width / 2;
    double subHeight = height / 2;

    // Adjust the child node positions relative to the parent node
    node->children[0] = std::make_unique<Node>(node->x - subWidth / 2, node->y - subHeight / 2, subWidth, subHeight);
    node->children[1] = std::make_unique<Node>(node->x + subWidth / 2, node->y - subHeight / 2, subWidth, subHeight);
    node->children[2] = std::make_unique<Node>(node->x - subWidth / 2, node->y + subHeight / 2, subWidth, subHeight);
    node->children[3] = std::make_unique<Node>(node->x + subWidth / 2, node->y + subHeight / 2, subWidth, subHeight);


    // Debug output to print node information after splitting
    std::cout << "Splitting Node - Parent: X: " << x << ", Y: " << y << ", Width: " << width << ", Height: " << height << std::endl;
    for (int i = 0; i < 4; ++i) 
    {
        std::cout << "Child " << i << " - X: " << node->children[i]->x << ", Y: " << node->children[i]->y << ", Width: " << node->children[i]->width << ", Height: " << node->children[i]->height << std::endl;
    }
}



// Get the quadrant that the node will live in
int Quadtree::getQuadrant(Node* node, double px, double py) {
    if (px < node->x && py < node->y) 
    {
        return 0; // Top-left
    }
    else if (px >= node->x && py < node->y) 
    {
        return 1; // Top-right
    }
    else if (px < node->x && py >= node->y) 
    {
        return 2; // Bottom-left
    }
    else 
    {
        return 3; // Bottom-right
    }
}


void Quadtree::collectParticles(Node* node) {
    if (node->isLeaf) {
        if (node->particle != nullptr) {
            this->particles.push_back(*(node->particle));
        }
    }
    else {
        for (int i = 0; i < 4; ++i) {
            if (node->children[i] != nullptr) {
                collectParticles(node->children[i].get());
            }
        }
    }
}


// Populate our space with a random assortment of particles
void Quadtree::generateRandomParticles(int numParticles, double particleMass, double simX, double simY, double simWidth, double simHeight) {
    // Clear existing particles
    particles.clear();

    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> randX(simX - simWidth / 2, simX + simWidth / 2);
    std::uniform_real_distribution<double> randY(simY - simHeight / 2, simY + simHeight / 2);

    // Using OpenMP, spit the particle out at a random place. This speeds up insertion if there are say 1M particles
#pragma omp parallel for
    for (int i = 0; i < numParticles; ++i) {
        double px = randX(gen);
        double py = randY(gen);

        Particle* particle = new Particle(px, py, particleMass);
        insert(Quadtree::root.get(), particle);

        // Debug output
        std::cout << "Generated Particle - X: " << particle->x << ", Y: " << particle->y << ", Mass: " << particle->mass << std::endl;
    }
}









// Helper function to get us particles. Try and insert them inside the root
void Quadtree::seedParticles(int numParticles, double mass, double simX, double simY, double simWidth, double simHeight) {
    generateRandomParticles(numParticles, mass, simX, simY, simWidth, simHeight);
}





// Get our particles return them to main
std::vector<Particle> Quadtree::getParticles() {
    // Clear existing particles before collecting
    this->particles.clear();
    collectParticles(this->root.get());
    return this->particles;
}



void Quadtree::updateParticlesAfterForces(std::vector<Particle>& particles, double deltaTime) {
    updateMass(this->root.get());

#pragma omp parallel for
    for (int i = 0; i < particles.size(); ++i) {
        Particle& particle = particles[i];

        // Debug output
        //std::cout << "Particle Before Update - X: " << particle.x << ", Y: " << particle.y << std::endl;

        // Update forces in the Quadtree
        this->root->updateForce(particle, this->theta, forceScale);

        // Debug output
        //std::cout << "Forces after update - X: " << particle.forceX << ", Y: " << particle.forceY << std::endl;

        // Update particle position based on velocity
        particle.updatePosition(deltaTime);

        // Debug output
        //std::cout << "Particle after update - X: " << particle.x << ", Y: " << particle.y << std::endl;
    }
}
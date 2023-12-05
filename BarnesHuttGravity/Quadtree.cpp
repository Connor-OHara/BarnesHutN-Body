// Quadtree.cpp
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

double forceScale = 1000.0;  // Adjust this scaling factor as needed



void Quadtree::updateMass(Node* node) {
    if (node->isLeaf) {
        if (node->particle != nullptr) {
            node->mass = node->particle->mass;
        }
        else {
            node->mass = 0.0;
        }
    }
    else {
        node->mass = 0.0;
        for (int i = 0; i < 4; ++i) {
            if (node->children[i] != nullptr) {
                updateMass(node->children[i].get());
                node->mass += node->children[i]->mass;
            }
        }
    }
}



void Quadtree::insert(Node* node, Particle* particle) {
    // Existing particle at this node
    if (node->isLeaf && node->particle != nullptr) {
        // Split the node only if the particle and the existing one are at different positions
        if (node->particle->x != particle->x || node->particle->y != particle->y) {
            node->isLeaf = false;
            split(node, particle->x, particle->y, node->width, node->height);
            moveParticleToChild(node, node->particle);
        }
        // Move the new particle to the appropriate child node
        moveParticleToChild(node, particle);
    }
    // No existing particle, insert the new one
    else if (node->isLeaf && node->particle == nullptr) {
        node->particle = particle;
    }
    // Internal node, move the new particle to the appropriate child node
    else {
        int quadrant = getQuadrant(node, particle->x, particle->y);

        if (node->children[quadrant] == nullptr) {
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

    if (node->children[quadrant] == nullptr) {
        double subX = node->x + (quadrant % 2) * node->width / 2;
        double subY = node->y + (quadrant / 2) * node->height / 2;
        node->children[quadrant] = std::make_unique<Node>(subX + node->width / 4, subY + node->height / 4, node->width / 2, node->height / 2);
    }

    insert(node->children[quadrant].get(), particle);
}

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
    for (int i = 0; i < 4; ++i) {
        std::cout << "Child " << i << " - X: " << node->children[i]->x << ", Y: " << node->children[i]->y << ", Width: " << node->children[i]->width << ", Height: " << node->children[i]->height << std::endl;
    }
}




int Quadtree::getQuadrant(Node* node, double px, double py) {
    if (px < node->x && py < node->y) {
        return 0; // Top-left
    }
    else if (px >= node->x && py < node->y) {
        return 1; // Top-right
    }
    else if (px < node->x && py >= node->y) {
        return 2; // Bottom-left
    }
    else {
        return 3; // Bottom-right
    }
}

void Quadtree::collectParticles(Node* node, std::vector<Particle>& particles) {
    if (node->isLeaf) {
        if (node->particle != nullptr) {
            particles.push_back(*(node->particle));
        }
    }
    else {
        for (int i = 0; i < 4; ++i) {
            if (node->children[i] != nullptr) {
                collectParticles(node->children[i].get(), particles);
            }
        }
    }
}


void Quadtree::generateRandomParticles(Node* node, int numParticles, double particleMass, double simX, double simY, double simWidth, double simHeight) {
    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> randX(simX - simWidth / 2, simX + simWidth / 2);
    std::uniform_real_distribution<double> randY(simY - simHeight / 2, simY + simHeight / 2);

#pragma omp parallel for
    for (int i = 0; i < numParticles; ++i) {
        double px = randX(gen);
        double py = randY(gen);

        Particle* particle = new Particle(px, py, particleMass);
        insert(node, particle);

        // Debug output
        std::cout << "Generated Particle - X: " << particle->x << ", Y: " << particle->y << ", Mass: " << particle->mass << std::endl;
    }
}









void Quadtree::seedParticles(int numParticles, double mass, double simX, double simY, double simWidth, double simHeight) {
    generateRandomParticles(root.get(), numParticles, mass, root->x, root->y, root->width, root->height);
}




std::vector<Particle> Quadtree::getParticles() {
    std::vector<Particle> particles;
    collectParticles(root.get(), particles);
    return particles;
}



void Quadtree::updateParticlesAfterForces(std::vector<Particle>& particles, double deltaTime) {
    updateMass(root.get());

#pragma omp parallel for
    for (int i = 0; i < particles.size(); ++i) {
        Particle& particle = particles[i];

        // Debug output
        std::cout << "Particle Before Update - X: " << particle.x << ", Y: " << particle.y << std::endl;

        // Update forces in the Quadtree
        root->updateForce(particle, theta, forceScale);

        // Debug output
        std::cout << "Forces after update - X: " << particle.forceX << ", Y: " << particle.forceY << std::endl;

        // Update particle position based on velocity
        particle.updatePosition(deltaTime);

        // Debug output
        std::cout << "Particle after update - X: " << particle.x << ", Y: " << particle.y << std::endl;
    }
}
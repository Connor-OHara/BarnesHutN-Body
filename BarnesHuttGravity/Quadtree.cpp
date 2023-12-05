// Quadtree.cpp
#include "Quadtree.h"
#include "Node.h"
#include "Particle.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>



Quadtree::Quadtree(double x, double y, double width, double height, double theta) : root(new Node(x, y, width, height)), theta(theta) {}

Quadtree::~Quadtree() {}

double forceScale = 1.0;  // Adjust this scaling factor as needed


void Quadtree::insert(Node* node, Particle* particle) {
    node->mass += particle->mass;
    node->x = (node->x * (node->mass - particle->mass) + particle->x * particle->mass) / node->mass;
    node->y = (node->y * (node->mass - particle->mass) + particle->y * particle->mass) / node->mass;

    if (node->isLeaf) {
        if (node->particle == nullptr) {
            node->particle = particle;
        }
        else {
            if (node->x == particle->x && node->y == particle->y) {
                node->isLeaf = false;
                split(node, particle->x, particle->y, node->width, node->height);

                int existingParticleQuadrant = getQuadrant(node, node->particle->x, node->particle->y);
                insert(node->children[existingParticleQuadrant].get(), node->particle);
                node->particle = nullptr;

                int newParticleQuadrant = getQuadrant(node, particle->x, particle->y);
                insert(node->children[newParticleQuadrant].get(), particle);
            }
            else {
                node->isLeaf = false;
                split(node, particle->x, particle->y, node->width, node->height);

                int existingParticleQuadrant = getQuadrant(node, node->particle->x, node->particle->y);
                insert(node->children[existingParticleQuadrant].get(), node->particle);
                node->particle = nullptr;

                int newParticleQuadrant = getQuadrant(node, particle->x, particle->y);
                insert(node->children[newParticleQuadrant].get(), particle);
            }
        }
    }
    else {
        int quadrant = getQuadrant(node, particle->x, particle->y);

        if (node->children[quadrant] == nullptr) {
            double subX = node->x + (quadrant % 2) * node->width / 2;
            double subY = node->y + (quadrant / 2) * node->height / 2;
            node->children[quadrant] = std::make_unique<Node>(subX + node->width / 4, subY + node->height / 4, node->width / 2, node->height / 2);
        }

        insert(node->children[quadrant].get(), particle);
    }
}



void Quadtree::split(Node* node, double x, double y, double width, double height) {
    double subWidth = width / 2;
    double subHeight = height / 2;

    node->children[0] = std::make_unique<Node>(x + subWidth / 2, y + subHeight / 2, subWidth, subHeight);
    node->children[1] = std::make_unique<Node>(x + 3 * subWidth / 2, y + subHeight / 2, subWidth, subHeight);
    node->children[2] = std::make_unique<Node>(x + subWidth / 2, y + 3 * subHeight / 2, subWidth, subHeight);
    node->children[3] = std::make_unique<Node>(x + 3 * subWidth / 2, y + 3 * subHeight / 2, subWidth, subHeight);
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

void Quadtree::generateRandomParticles(Node* node, int numParticles) {
    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> randX(node->x - node->width / 2, node->x + node->width / 2);
    std::uniform_real_distribution<double> randY(node->y - node->height / 2, node->y + node->height / 2);

    for (int i = 0; i < numParticles; ++i) {
        double px = randX(gen);
        double py = randY(gen);

        // Ensure particle stays within the node's boundaries
        px = std::max(node->x - node->width / 2, std::min(node->x + node->width / 2, px));
        py = std::max(node->y - node->height / 2, std::min(node->y + node->height / 2, py));

        Particle* particle = new Particle(px, py, .1);
        insert(node, particle);
    }
}

void Quadtree::seedParticles(int numParticles) {
    generateRandomParticles(root.get(), numParticles);
}

std::vector<Particle> Quadtree::getParticles() {
    std::vector<Particle> particles;
    collectParticles(root.get(), particles);
    return particles;
}

void Quadtree::updateParticlesAfterForces(std::vector<Particle>& particles, double deltaTime) {
    // Update forces and positions directly in the provided vector
    for (auto& particle : particles) {
        // Debug output for initial forces
        std::cout << "Initial Forces - X: " << particle.forceX << ", Y: " << particle.forceY << std::endl;

        // Update forces based on the quadtree
        root->updateForce(particle, theta, forceScale);

        // Debug output for forces after update
        std::cout << "Forces after update - X: " << particle.forceX << ", Y: " << particle.forceY << std::endl;



        // Update position based on the calculated forces
        particle.updatePosition(deltaTime);

        // Debug output for updated position
        //std::cout << "PART Updated Velocity - X: " << particle.velocityX << ", Y: " << particle.velocityY << std::endl;

        // Debug output for updated position
        //std::cout << "PART Updated Position - X: " << particle.x << ", Y: " << particle.y << std::endl;
    }
}



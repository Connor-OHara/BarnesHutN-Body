/*
Author: Connor O'Hara
Class: ECE4122 
Last Date Modified: 12/8/23
Description:
Header file for the Quadtree, which contains the entirety of the simulation backend
*/

#pragma once
#ifndef QUADTREE_H
#define QUADTREE_H

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Node.h"
#include "Particle.h"

class Quadtree {
public:
    std::unique_ptr<Node> root;
    double theta;
    std::vector<Particle> particles;

    Quadtree(double x, double y, double width, double height, double theta);
    ~Quadtree();

    void updateMass(Node* node);
    void insert(Node* node, Particle* particle);
    void moveParticleToChild(Node* node, Particle* particle);
    void split(Node* node, double x, double y, double width, double height);
    int getQuadrant(Node* node, double px, double py);

    void collectParticles(Node* node);
    void generateRandomParticles(int numParticles, double particleMass, double simX, double simY, double simWidth, double simHeight);
    // Add this declaration inside the Quadtree class
    void seedParticles(int numParticles, double mass, double simX, double simY, double simWidth, double simHeight);

    std::vector<Particle>& getParticles();
    void updateParticlesAfterForces(std::vector<Particle>& particles, double deltaTime);
    
    sf::FloatRect getBoundingBox() const {
        return root->getBoundingBox();
    }

};

#endif // QUADTREE_H
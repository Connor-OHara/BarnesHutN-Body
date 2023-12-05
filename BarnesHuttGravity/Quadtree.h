// Quadtree.h
#pragma once
#ifndef QUADTREE_H
#define QUADTREE_H

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Node.h"

class Node; // Forward declaration
class Particle; // Forward declaration

class Quadtree {
public:
    std::unique_ptr<Node> root;
    double theta;

    Quadtree(double x, double y, double width, double height, double theta);
    ~Quadtree();
    void insert(Node* node, Particle* particle);
    void generateRandomParticles(Node* node, int numParticles);
    void split(Node* node, double x, double y, double width, double height);
    int getQuadrant(Node* node, double px, double py);

    
    void collectParticles(Node* node, std::vector<Particle>& particles);

    
    std::vector<Particle> getParticles();

    void updateQuadtree(double deltaTime, int numIterations);

    //Get particles after apply forces
    void updateParticlesAfterForces(std::vector<Particle>& particles, double deltaTime);



    void seedParticles(int numParticles);



    sf::FloatRect getBoundingBox() const {
        return root->getBoundingBox();
    }
};
#endif // QUADTREE_H
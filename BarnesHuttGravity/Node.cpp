/*
Author: Connor O'Hara
Class: ECE4122
Last Date Modified: 12/8/23
Description:
Our Node, which represents a sector of space in our quadtree.
*/


#include "Node.h"
#include <cmath>
#include <iostream>
#include "Particle.h"

Node::Node(double x, double y, double width, double height)
    : x(x), y(y), width(width), height(height), mass(0.0), isLeaf(true), particle(nullptr) {
    for (int i = 0; i < 4; ++i)
    {
        children[i] = nullptr;
    }
}

Node::~Node() {
    for (int i = 0; i < 4; ++i)
    {
        children[i].reset(); // Reset the std::unique_ptr to release ownership and delete the Node
    }
}

void Node::updateForce(Particle& target, double theta, double forceScale) {
    double dx = target.x - x;
    double dy = target.y - y;
    double distance = std::sqrt(dx * dx + dy * dy);

    // Introduce a minimum distance threshold
    double minDistance = 1e-5;
    distance = std::max(distance, minDistance);

    double ratio = width / distance;

    //std::cout << "Node - X: " << x << ", Y: " << y << ", Width: " << width << ", Height: " << height << std::endl;
    //std::cout << "Particle - X: " << target.x << ", Y: " << target.y << ", Mass: " << target.mass << std::endl;
    //std::cout << "Distance: " << distance << std::endl;

    if (isLeaf || (ratio < theta)) {
        double force = (G * target.mass * mass) / (distance * distance);
        //std::cout << "FORCE: " << force << std::endl;
        //std::cout << "G: " << G << ", Target Mass: " << target.mass << ", Node Mass: " << mass << ", Distance: " << distance << std::endl;
        //std::cout << "dx: " << dx << ", dy: " << dy << ", distance^2: " << distance * distance << std::endl;

        // Adjust the force scale
        force *= forceScale;

        // Cap the forces to prevent excessively large values
        double maxForce = 1e10;
        double scaledForceX = std::min(std::max(force * (dx / distance), -maxForce), maxForce);
        double scaledForceY = std::min(std::max(force * (dy / distance), -maxForce), maxForce);

        //std::cout << "Scaled Force - X: " << scaledForceX << ", Y: " << scaledForceY << std::endl;

        // Apply the scaled forces to the target particle
        target.forceX += scaledForceX;
        target.forceY += scaledForceY;

        //std::cout << "Force applied Totale :X: " << target.forceX << ", Y: " << target.forceY << std::endl;
    }
    else {
        //std::cout << "Barnes-Hut criterion not satisfied. Skipping further calculations." << std::endl;
    }

    if (!isLeaf) {
        // Recursively update forces for each child node
        for (int i = 0; i < 4; ++i) {
            if (children[i] != nullptr) {
                children[i]->updateForce(target, theta, forceScale);
            }
        }
    }
}


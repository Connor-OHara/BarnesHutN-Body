
#define _USE_MATH_DEFINES

#include "Node.h"
#include <stdexcept>
#include <stdio.h>
#include <iostream>
#include "Vect.h"
#include <omp.h>
#include <cmath>
#include <math.h>
#include <stack>
#include "Particle.h"
#include "Quadtree.h"




Node::Node(double x, double y, double width, double height) : x(x), y(y), width(width), height(height), mass(0.0), isLeaf(true), particle(nullptr) {
    for (int i = 0; i < 4; ++i) {
        children[i] = nullptr;
    }
}

Node::~Node() {
    for (int i = 0; i < 4; ++i) {
        children[i].reset(); // Reset the std::unique_ptr to release ownership and delete the Node
    }
}


void Node::updateForce(Particle& target, double theta, double forceScale) {
    // Debug output
    //std::cout << "Leaf Particle Forces - X: " << target.forceX << ", Y: " << target.forceY << std::endl;

    if (isLeaf) {
        if (particle != nullptr && &target != particle) {
            // Calculate and apply forces to the target particle...
            double dx = target.x - x;
            double dy = target.y - y;
            double distance = std::sqrt(dx * dx + dy * dy);

            const double minDistance = 1e-3; // Adjust as needed
            if (distance > minDistance) {
                double force = (G * target.mass * mass) / (distance * distance);

                // Scale the force components
                double scaledForceX = forceScale * force * (dx / distance);
                double scaledForceY = forceScale * force * (dy / distance);

                // Apply the scaled forces to the target particle
                target.forceX -= scaledForceX;
                target.forceY -= scaledForceY;

                // Debug output
                //std::cout << "Leaf Particle Forces - X: " << scaledForceX << ", Y: " << scaledForceY << std::endl;

                // Also, update forces for the particle associated with the leaf node
                double dx_leaf = particle->x - x;
                double dy_leaf = particle->y - y;
                double distance_leaf = std::sqrt(dx_leaf * dx_leaf + dy_leaf * dy_leaf);

                if (distance_leaf != 0.0) {
                    double force_leaf = (G * target.mass * mass) / (distance_leaf * distance_leaf);

                    // Scale the force components
                    double scaledForceX_leaf = forceScale * force_leaf * (dx_leaf / distance_leaf);
                    double scaledForceY_leaf = forceScale * force_leaf * (dy_leaf / distance_leaf);

                    // Apply the scaled forces to the particle associated with the leaf node
                    particle->forceX += scaledForceX_leaf;
                    particle->forceY += scaledForceY_leaf;

                    // Debug output
                    //std::cout << "Leaf Particle Forces (Node Particle) - X: " << scaledForceX_leaf << ", Y: " << scaledForceY_leaf << std::endl;
                }
            }
        }
    }
    else {
        double ratio = std::max(std::max(width, height) / std::sqrt((x - target.x) * (x - target.x) + (y - target.y) * (y - target.y)), theta);

        if (ratio < theta) {
            double dx = target.x - x;
            double dy = target.y - y;
            double distance = std::sqrt(dx * dx + dy * dy);

            const double minDistance = 1e-3; // Adjust as needed
            if (distance > minDistance) {
                double force = (G * target.mass * mass) / (distance * distance);

                // Scale the force components
                double scaledForceX = forceScale * force * (dx / distance);
                double scaledForceY = forceScale * force * (dy / distance);

                // Apply the scaled forces to the target particle
                target.forceX -= scaledForceX;
                target.forceY -= scaledForceY;

                // Debug output
                //std::cout << "Intermediate Forces - X: " << scaledForceX << ", Y: " << scaledForceY << std::endl;

                // Also, update forces for the particle associated with the current node
                double dx_node = particle->x - x;
                double dy_node = particle->y - y;
                double distance_node = std::sqrt(dx_node * dx_node + dy_node * dy_node);

                if (distance_node != 0.0) {
                    double force_node = (G * target.mass * mass) / (distance_node * distance_node);

                    // Scale the force components
                    double scaledForceX_node = forceScale * force_node * (dx_node / distance_node);
                    double scaledForceY_node = forceScale * force_node * (dy_node / distance_node);

                    // Apply the scaled forces to the particle associated with the current node
                    particle->forceX += scaledForceX_node;
                    particle->forceY += scaledForceY_node;

                    // Debug output
                    //std::cout << "Intermediate Forces (Node Particle) - X: " << scaledForceX_node << ", Y: " << scaledForceY_node << std::endl;
                }
            }
        }
        else {
            for (int i = 0; i < 4; ++i) {
                if (children[i] != nullptr) {
                    // Recursively update forces with the scaling factor
                    children[i]->updateForce(target, theta, forceScale);
                }
            }
        }
    }
}
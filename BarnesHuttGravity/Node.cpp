
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


void Node::updateForce(Particle* target, double theta) {
    if (isLeaf) {
        if (particle != nullptr && particle != target) {
            double dx = target->x - x;
            double dy = target->y - y;
            double distance = std::sqrt(dx * dx + dy * dy);

            if (distance != 0.0) {
                double force = (G * target->mass * mass) / (distance * distance);

                target->forceX -= force * (dx / distance);
                target->forceY -= force * (dy / distance);
            }
        }
    }
    else {
        double ratio = std::max(std::max(width, height) / std::sqrt((x - target->x) * (x - target->x) + (y - target->y) * (y - target->y)), theta);

        if (ratio < theta) {
            double dx = target->x - x;
            double dy = target->y - y;
            double distance = std::sqrt(dx * dx + dy * dy);

            if (distance != 0.0) {
                double force = (G * target->mass * mass) / (distance * distance);

                target->forceX -= force * (dx / distance);
                target->forceY -= force * (dy / distance);
            }
        }
        else {
            for (int i = 0; i < 4; ++i) {
                if (children[i] != nullptr) {
                    children[i]->updateForce(target, theta);
                }
            }
        }
    }
}
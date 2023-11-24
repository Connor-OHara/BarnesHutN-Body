// Quadtree.h
#pragma once
#include <memory>

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
};

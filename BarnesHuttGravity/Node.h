// Node.h
#pragma once
#include <memory>

class Particle; // Forward declaration

class Node {
public:
    double x, y; // Center of mass position
    double width, height;
    double mass;
    bool isLeaf;
    Particle* particle;
    std::unique_ptr<Node> children[4];

    // Gravitational Constant
    static constexpr double G = 6.67428e-11; // Gravitational Constant
    Node(double x, double y, double width, double height);
    ~Node();
    void updateForce(Particle* target, double theta);


};

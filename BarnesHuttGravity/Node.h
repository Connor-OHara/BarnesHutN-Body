// Node.h

#ifndef NODE_H
#define NODE_H

#pragma once
#include <memory>
#include <SFML/Graphics.hpp>

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
    void updateForce(Particle& target, double theta, double forceScale);


    sf::FloatRect getBoundingBox() const {
        return sf::FloatRect(static_cast<float>(x - width / 2), static_cast<float>(y - height / 2), static_cast<float>(width), static_cast<float>(height));

    }

};

#endif // NODE_H
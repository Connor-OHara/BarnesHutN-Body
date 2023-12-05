/*
    Connor O'Hara
    Main Program for Barnes-Hut simulation.

    This shows an approximation of an N-body problem using the Barnes-Hut octree
    implementation for O(nlog(n)) time complexity. It should not be used for
    precise calculation for specific bodies but will suffice for observing system-level
    effects.

    // The Wikipedia page on Barnes-Hut gave pseudocode reference for implementation
    // Pseudocode ref: https://beltoforion.de/en/barnes-hut-galaxy-simulator/
    // More pseudocode: https://people.eecs.berkeley.edu/~demmel/cs267/lecture26/lecture26.html#link_4
    // This pseudocode was used to help understand the structure of Barnes Hut.
    // I additionally looked at https://www.geeksforgeeks.org/quad-tree/# for quadtree implementations
*/
#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include "Quadtree.h"
#include "Node.h"
#include "Particle.h"
#include <iostream>



void drawBoundingBoxes(sf::RenderWindow& window, Node* node, sf::Vector2f parentCenter = sf::Vector2f(0.0f, 0.0f), sf::Vector2f parentSize = sf::Vector2f(0.0f, 0.0f), bool isRoot = false) {
    if (node) {
        // Draw the bounding box for the current node
        sf::FloatRect boundingBox = node->getBoundingBox();
        sf::RectangleShape rect(sf::Vector2f(boundingBox.width, boundingBox.height));
        rect.setPosition(boundingBox.left, boundingBox.top);
        rect.setFillColor(sf::Color::Transparent);

        // Set the outline color
        if (isRoot) {
            // Root node color (green border)
            rect.setOutlineColor(sf::Color::Green);
        }
        else {
            // Non-root nodes color (blue border)
            rect.setOutlineColor(sf::Color::Blue);
        }

        rect.setOutlineThickness(2.0f);
        window.draw(rect);

        // Recursively draw bounding boxes for children
        for (int i = 0; i < 4; ++i) {
            if (node->children[i]) {
                // Calculate child position
                sf::Vector2f childPosition;
                switch (i) {
                case 0: // Top left
                    childPosition = sf::Vector2f(boundingBox.left, boundingBox.top);
                    break;
                case 1: // Top right
                    childPosition = sf::Vector2f(boundingBox.left + boundingBox.width / 2.0f, boundingBox.top);
                    break;
                case 2: // Bottom left
                    childPosition = sf::Vector2f(boundingBox.left, boundingBox.top + boundingBox.height / 2.0f);
                    break;
                case 3: // Bottom right
                    childPosition = sf::Vector2f(boundingBox.left + boundingBox.width / 2.0f, boundingBox.top + boundingBox.height / 2.0f);
                    break;
                }

                // Recursively draw bounding boxes for children
                drawBoundingBoxes(window, node->children[i].get(), childPosition, sf::Vector2f(boundingBox.width / 2.0f, boundingBox.height / 2.0f), false);
            }
        }
    }
}


/*
 * This is the main function that acts as a dispatcher for the subprocesses that will manage
 * The running of the simulation.
 */
int main() {
    // Initialize quadtree with the desired parameters
    double simulationX = 0.0;
    double simulationY = 0.0;
    double simulationWidth = 1400.0;
    double simulationHeight = 950.0;
    Quadtree quadtree(simulationX, simulationY, simulationWidth, simulationHeight, 0.1);

    // Seed the screen with particles
    quadtree.seedParticles(2, 100000000, simulationX, simulationY, simulationWidth, simulationHeight);

    // Create SFML window
    sf::RenderWindow window(sf::VideoMode(1400, 950), "Barnes-Hut N-Body Simulation");

    // Create SFML circle shape for particles
    sf::CircleShape particleShape(5.0f);
    particleShape.setFillColor(sf::Color::White);

    // Create an SFML view
    sf::View view(sf::FloatRect(0, 0, 1400, 950));
    window.setView(view);

    // Center the view on the particles initially
    sf::FloatRect boundingBox = quadtree.getBoundingBox();

    view.setCenter(boundingBox.left + boundingBox.width / 2, boundingBox.top + boundingBox.height / 2);
    window.setView(view);

    // Variables for click-and-drag
    bool isDragging = false;
    sf::Vector2i lastMousePosition;

    std::cout << "Finished building initial conditions: " << quadtree.getParticles().size() << std::endl;

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();


            // Handle zoom in and out
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) {
                    view.zoom(0.9f); // Zoom in
                }
                else if (event.mouseWheelScroll.delta < 0) {
                    view.zoom(1.1f); // Zoom out
                }
                window.setView(view);
            }

            // Handle click-and-drag for panning
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                isDragging = true;
                lastMousePosition = sf::Mouse::getPosition(window);
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                isDragging = false;
            }
        }

        // Update click-and-drag panning
        if (isDragging) {
            sf::Vector2i currentMousePosition = sf::Mouse::getPosition(window);
            sf::Vector2f delta = window.mapPixelToCoords(currentMousePosition) - window.mapPixelToCoords(lastMousePosition);
            view.move(-delta);
            window.setView(view);

            lastMousePosition = currentMousePosition;
        }

        // Get particles from the quadtree
        std::vector<Particle> particles = quadtree.getParticles();


        // Print positions of some particles
        std::cout << "Particle positions: ";
        for (int i = 0; i < std::min(5, static_cast<int>(particles.size())); ++i) {
            std::cout << "Particle " << i << " - X: " << particles[i].x << ", Y: " << particles[i].y << " | ";
        }
        std::cout << std::endl;

        // Assuming particles is a vector of Particle in your main code
        quadtree.updateParticlesAfterForces(particles, 100.0);



        // Print forces on the first particle
        if (!particles.empty()) {
            Particle& firstParticle = particles[0];
            //std::cout << "First Particle Position - X: " << firstParticle.x << ", Y: " << firstParticle.y << ", ForceX: " << firstParticle.forceX << ", ForceY: " << firstParticle.forceY << ", VelocityX: " << firstParticle.velocityX << " VelocityY: " << firstParticle.velocityY << std::endl;
        }

        // Clear the window
        window.clear();

        // Draw particles
        for (const auto& particle : particles) {
            // Set particle shape position
            particleShape.setPosition(static_cast<float>(particle.x), static_cast<float>(particle.y));

            // Draw the particle
            window.draw(particleShape);
        }

        /*
        // Draw force vectors for each particle
        for (const auto& particle : particles) {
            sf::Vertex forceVector[] = {
                sf::Vertex(sf::Vector2f(static_cast<float>(particle.x), static_cast<float>(particle.y))),
                sf::Vertex(sf::Vector2f(static_cast<float>(particle.x + particle.forceX), static_cast<float>(particle.y + particle.forceY)))
            };

            window.draw(forceVector, 2, sf::Lines);
        }
        */
        // Draw bounding box for the root node with green color
        drawBoundingBoxes(window, quadtree.root.get(), sf::Vector2f(0.0f, 0.0f), sf::Vector2f(simulationWidth, simulationHeight), true); // Adjust the parameters accordingly

        // Display the window
        window.display();
    }




    return 0;
}

/*
	Connor O'Hara
	Main Program for Barnes-Hut simulation.

	This shows an approximation of a N-body problem using the Barnes-Hut octree
	implementation for O(nlog(n)) time complexity. It should not be used for 
	precise calulation for specific bodies, but will suffice for observing system-level 
	effects.



	// The wikipedia page on barnes-Hut gave psuedocode reference for implementation
	// psuedocoderef: https://beltoforion.de/en/barnes-hut-galaxy-simulator/
	// more psuedocode: https://people.eecs.berkeley.edu/~demmel/cs267/lecture26/lecture26.html#link_4
	// This psuedocode was used to help understand the structure of Barnes Hut.
	// I additionally looked at https://www.geeksforgeeks.org/quad-tree/# for quadtree implementations



*/
#define _USE_MATH_DEFINES


#include <SFML/Graphics.hpp>
#include "Quadtree.h"
#include "Node.h"
#include "Particle.h"
#include <iostream>
/*
* This is the main function that acts as a dispatcher for the subprocesses that will manage 
* The running of the simulation. 
*/
int main() {
    // Initialize quadtree with the desired parameters
    Quadtree quadtree(0, 0, 1400, 950, 0.5);

    // Seed the screen with particles
    quadtree.seedParticles(100);

    // Create SFML window
    sf::RenderWindow window(sf::VideoMode(1400, 950), "Barnes-Hut N-Body Simulation");

    // Create SFML circle shape for particles
    sf::CircleShape particleShape(2.0f);
    particleShape.setFillColor(sf::Color::White);

    std::cout << "Finished building initial conditions: " << quadtree.getParticles().size() << std::endl;

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear the window
        window.clear();

        // Update forces and positions in the quadtree
        quadtree.updateQuadtree(0.1, 10);  // Adjust time step and iterations as needed

        // Get particles from the quadtree
        std::vector<Particle> particles = quadtree.getParticles();

        // Draw particles
        for (const auto& particle : particles) {
            // Set particle shape position
            particleShape.setPosition(particle.x, particle.y);

            // Draw the particle
            window.draw(particleShape);
        }

        // Display the window
        window.display();
    }

    return 0;
}

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


#include <stdio.h>
#include <omp.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include "Node.h"



/*
* This is the main function that acts as a dispatcher for the subprocesses that will manage 
* The running of the simulation. 
*/
int main() {
    try {
        std::cout << "Building Tree\n";

        // Initialize the global root node
        Node rootNode(Vect(0, 0), Vect(100, 100), nullptr);
        Node::root = &rootNode;

        std::cout << std::fixed << std::setprecision(2);

        // Create a galaxy-like dataset of points
        const int numPoints = 1000000;
        const double centerX = 50.0;
        const double centerY = 50.0;
        const double galaxyRadius = 100.0;

        for (int i = 0; i < numPoints; ++i) {
            double angle = 2.0 * M_PI * static_cast<double>(i) / numPoints;
            double radius = galaxyRadius * static_cast<double>(i) / numPoints;

            double x = centerX + radius * std::cos(angle);
            double y = centerY + radius * std::sin(angle);

            auto particle = std::make_shared<Particle>(x, y, 10.0); // Adjust mass as needed
            rootNode.Add(particle);
        }

        // Print the details of the galaxy particles
        std::cout << "Particle Count: " << rootNode.particleCount << std::endl;

        // Print final details
        double rootMass = rootNode.mass;
        std::cout << "Root Mass: " << rootMass << std::endl;
        std::cout << "Particle Count: " << rootNode.particleCount << std::endl;
    }
    catch (const std::exception& exec) {
        std::cout << "Error: " << exec.what() << std::endl;
    }
    catch (...) {
        std::cout << "Unknown Error" << std::endl;
    }

    return 0;
}
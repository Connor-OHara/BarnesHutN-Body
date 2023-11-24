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
#include <vector>
#include "Quadtree.h"


#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>
#include <memory>
#include "particle.h"



/*
* This is the main function that acts as a dispatcher for the subprocesses that will manage 
* The running of the simulation. 
*/
int main() {
    try {
        Quadtree quadtree(0, 0, 100, 100, 0.5);

        // Generate and insert random particles
        quadtree.generateRandomParticles(quadtree.root.get(), 100);

        // Print out random particles and forces
        for (int i = 0; i < 100; ++i) {
            // Generate random positions for each particle
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<double> randX(quadtree.root->x - quadtree.root->width / 2, quadtree.root->x + quadtree.root->width / 2);
            std::uniform_real_distribution<double> randY(quadtree.root->y - quadtree.root->height / 2, quadtree.root->y + quadtree.root->height / 2);

            Particle randomParticle(randX(gen), randY(gen), 0.1);
            quadtree.root->updateForce(&randomParticle, quadtree.theta);
            std::cout << "Random Particle " << i + 1 << ": (" << randomParticle.x << ", " << randomParticle.y
                << "), Force: (" << randomParticle.forceX << ", " << randomParticle.forceY << ")" << std::endl;
        }

        return 0;
    }
    catch (const std::exception& exec) {
        std::cout << "Error: " << exec.what() << std::endl;
    }
    catch (...) {
        std::cout << "Unknown Error" << std::endl;
    }

    return 0;
}

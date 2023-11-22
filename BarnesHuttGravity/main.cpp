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



#include <stdio.h>
#include <omp.h>
#include <iostream>
#include "Node.h"
#include <iomanip>



/*
* This is the main function that acts as a dispatcher for the subprocesses that will manage 
* The running of the simulation. 
*/
int main(int argc, char** argv) {
    try {
        std::cout << "Building Tree\n";

        // Initialize the global root node
        Node rootNode(Vect(0, 0), Vect(100, 100), nullptr);
        Node::root = &rootNode;

        std::cout << std::fixed << std::setprecision(2);

        auto rootpart  = std::make_shared<Particle>(45.0, 2.0, 100.0);
        auto rootpart2 = std::make_shared<Particle>(0.0, 6.0, 50.0);
        auto rootpart3 = std::make_shared<Particle>(2.0, 3.0, 75.0);

        std::cout << "Rootpart: X: " << std::fixed << std::setprecision(2) << rootpart->getX() << " Y: " << rootpart->getY() << std::endl;
        std::cout << "Rootpart2: X: " << std::fixed << std::setprecision(2) << rootpart2->getX() << " Y: " << rootpart2->getY() << std::endl;
        std::cout << "Rootpart3: X: " << std::fixed << std::setprecision(2) << rootpart3->getX() << " Y: " << rootpart3->getY() << std::endl;



        std::cout << "Particle Count: " <<  rootNode.particleCount << std::endl;

        rootNode.Add(rootpart);
        double rootMass = rootNode.mass;

        std::cout << "Root Mass: " << rootMass << std::endl;
        std::cout << "Particle Count: " << rootNode.particleCount << std::endl;

        rootNode.Add(rootpart2);
        rootMass = rootNode.mass;

        std::cout << "Root Mass: " << rootMass << std::endl;
        std::cout << "Particle Count: " << rootNode.particleCount << std::endl;

        rootNode.Add(rootpart3);

        std::cout << "Root Mass: " << rootNode.mass << std::endl;
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
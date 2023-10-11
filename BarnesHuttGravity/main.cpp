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

/*
* This is the main function that acts as a dispatcher for the subprocesses that will manage 
* The running of the simulation. 
*/
int main(int argc, char** argv) 
{
	try
	{
		std::cout << "Building Tree";

		//Reset Tree

		//Launch app



	}
	catch (std::exception& exec)
	{
		std::cout << "Error: " << exec.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown Error" << std::endl;
	}

	return 0;
}
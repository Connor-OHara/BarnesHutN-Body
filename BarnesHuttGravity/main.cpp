/*
	Connor O'Hara
	Main Program for Barnes-Hut simulation.

	This shows an approximation of a N-body problem using the Barnes-Hut octree
	implementation for O(nlog(n)) time complexity. It should not be used for 
	precise calulation for specific bodies, but will suffice for observing system-level 
	effects.

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
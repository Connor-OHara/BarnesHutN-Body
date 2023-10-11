#include <stdio.h>
#include "BarnesHut.h"

/*
    This class will contain our Node, which itself contains reference
    to the entire BarnesHut tree.

    At a high level, here is the Barnes-Hut algorithm:

   1)  Build the Quadtree, using QuadtreeBuild
   2)  For each subsquare in the quadtree,
       compute the center of mass and total mass
       for all the particles it contains.
   3)  For each particle, traverse the tree
       to compute the force on it.
*/


/*
* procedure QuadtreeBuild
     Quadtree = {empty}
       For i = 1 to n          ... loop over all particles
         QuadInsert(i, root)   ... insert particle i in quadtree
       end for
       ... at this point, the quadtree may have some empty 
       ... leaves, whose siblings are not empty
       Traverse the tree (via, say, breadth first search), 
         eliminating empty leaves

*/

BarnesHut::BarnesHut()
{






}

void BarnesHut::quadTreeBuild()
{


    



}
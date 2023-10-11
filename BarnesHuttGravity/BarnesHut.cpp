#include <stdio.h>
#include "BarnesHut.h"

/*
    This class will contain our Node, which itself contains reference
    to the entire BarnesHut tree.
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


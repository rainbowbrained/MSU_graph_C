/*
*     The programm initializes a graph, then reads information about the graph from 
* input4.txt. Then it prints half of ribs with the biggest weight in a descending order.
*/

#include <stdio.h>
#include "graphlib.h"


int main(void)
{
    struct Graph my_graph;
    GraphInit(&my_graph);
    GraphGetInfo (&my_graph, "input4.txt");
    GraphSort (&my_graph, *SORT_WEIGHT);
    
    for (int i = my_graph.n_ribs - 1; i >= my_graph.n_ribs/2; i--) {
		printf ("start: %i | end: %i | weight %i \n", my_graph.table[i][0], 
		        my_graph.table[i][1], my_graph.table[i][2]);
	}
	
    GraphDeinit(&my_graph);
    
    return 0;
}

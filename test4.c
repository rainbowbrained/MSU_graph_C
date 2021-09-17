/*
*     The programm initializes a graph, then reads information about the graph from 
* stdin. If the stream is empty or programm has not got enough information it randomizes
* graph. Then it prints the number of node with the biggest degree (the number of ribs
* that begin or end in this node) and prints its degree.
* 
*     Ig the graph has several nodes with the same degree the function prints the first.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graphlib.h"

int my_rand(int lower, int upper, unsigned int key) 
{ 
	srand(key);
    int num = (rand() % (upper - lower + 1)) + lower; 
    return num;
} 

int main(void)
{
    struct Graph my_graph;
    GraphInit(&my_graph);
    printf ("Enter the integer number of ribs in one string, then enter 3 integer \n");
    printf ("numbers: start node, end node, weight (optional) in distinct strings \n");
    printf ("or ctrl+D to auto-initialize the graph's ribs\n");
    GraphGetInfo (&my_graph, "stdin");
    
    if (my_graph.n_nodes == 0) {
	    my_graph.n_nodes = my_rand(3, 20, time(0));
	    printf ("Number of nodes = %i\n", my_graph.n_nodes);
    }
    
    if (my_graph.n_ribs == 0) {
		int ribs = my_rand(3, 50, time(0));
	    for (int i = 0; i < ribs; i++) {
			int start = my_rand(0, my_graph.n_nodes, i);
			int end = my_rand(0, my_graph.n_nodes, i+1);
		    GraphAddRib (&my_graph, start+1, end+1, 0);
	    }
    GraphPrint(&my_graph);
	}
    
    int array[my_graph.n_nodes];
    for (int i = 0; i < my_graph.n_nodes; i++) {
		array[i] = 0;
	}
    
    for (int i = 0; i < my_graph.n_ribs; i++) {
		array[my_graph.table[i][0] - 1]++;
		array[my_graph.table[i][1] - 1]++;
	}
	
	int max1 = array[0], max2 = 0;
	for (int i = 1; i < my_graph.n_nodes; i++) {
		if (array[i] > max1) {
			max1 = array[i];
			max2 = i;
		}
	}
	
	printf ("The node №%i has the biggest degree %i\n", max2 + 1, max1);
    GraphDeinit(&my_graph);
    
    return 0;
}

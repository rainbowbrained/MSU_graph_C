/*
*     The programm initializes a graph, then reads information about the graph from 
* input1.txt. In case of failure it reads fron input3.txt and prints it's weight. 
* Then it bondes separate connected components in the graph into one connected graph 
* by adding 0 weight ribs. Then it makes a minimal spanning graph and prints it weight.
*     In the end the programm deinitializes the graph.
* 
*     ATTENTION! The number of nodes may fit in the integer, but still be so big that it
* would be impossible to create an array array[my_graph.n_nodes]. In this case user is
* not protected from malfunctions.
*/

#include <stdio.h>
#include "graphlib.h"

int main(void)
{
    struct Graph my_graph;
    GraphInit(&my_graph);
    GraphGetInfo (&my_graph, "input1.txt");
    
    if (my_graph.n_nodes == 0) {
		GraphGetInfo (&my_graph, "input3.txt");
	}
    
    int array[my_graph.n_nodes];
    int key = GraphConnectedComp (&my_graph, array, *WORKING_MODE);
    //GraphPrint (&my_graph);
    long int w = 0;
    
    for (int i = 0; i < my_graph.n_ribs; i++) {
        w += my_graph.table[i][2];
    }
    printf ("Old weight: %li\n", w);
   
    if (my_graph.n_nodes > 1) {
        for (int i = 0; i < my_graph.n_nodes - 1; i++) {
            if (array[i] == 0) {
                if (array[i+1] == 0) {
                    array[i] = key;
                    array[i+1] = key;
                    key++;
                    GraphAddRib (&my_graph, i+1, i+2, 0);
                } else {
                    array[i] = array[i+1];
                    GraphAddRib (&my_graph, i+1, i+2, 0);
                }
            } else if ((array[i+1] != array[i])&&(array[i+1] != 0)) {
                int temp1 = array[i], temp2 = array[i+1];
                for (int j = 0; j < my_graph.n_nodes; j++) {
                    if (array[j] == temp2) {
                        array[j] = temp1;
                    }
                }
                GraphAddRib (&my_graph, i+1, i+2, 0);
            } else if (array[i+1] == 0){
				array[i+1] = array[i];
				GraphAddRib (&my_graph, i+1, i+2, 0);
			}
        }
    }
    //GraphPrint (&my_graph);
    printf ("New weight: %li\n", GraphMinimal (&my_graph, *WORKING_MODE));
    //GraphPrint (&my_graph);
    
    GraphDeinit(&my_graph);
    
    return 0;
}


/*
*     The programm initializes a graph, then reads information about the graph from 
* input.txt. Then it makes two connected components with nodes with even order number 
* and nodes with non even order number by removing and adding ribs.
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
    GraphGetInfo (&my_graph, "input.txt");
    
    GraphPrint (&my_graph);
    
    for (int i = my_graph.n_ribs - 1; i >= 0; i--) {
		if (((my_graph.table[i][0] % 2 == 1)&&(my_graph.table[i][1] % 2 == 0))||
		        ((my_graph.table[i][0] % 2 == 1)&&(my_graph.table[i][1] % 2 == 0))) {
			GraphDeleteRib(&my_graph, i+1);
		}
    }
    
    int array[my_graph.n_nodes];
    int key = GraphConnectedComp (&my_graph, array, *WORKING_MODE);
    
    if (my_graph.n_nodes > 1) {
        for (int i = 0; i < my_graph.n_nodes - 2; i++) {
            if (array[i] == 0) {
                if (array[i+2] == 0) {
                    array[i] = key;
                    array[i+2] = key;
                    key++;
                    GraphAddRib (&my_graph, i+1, i+3, 0);
                } else {
                    array[i] = array[i+2];
                    GraphAddRib (&my_graph, i+1, i+3, 0);
                }
            } else if ((array[i+2] != array[i])&&(array[i+2] != 0)) {
                int temp1 = array[i], temp2 = array[i+2];
                for (int j = 0; j < my_graph.n_nodes; j++) {
                    if (array[j] == temp2) {
                        array[j] = temp1;
                    }
                }
                GraphAddRib (&my_graph, i+1, i+3, 0);
            } else if (array[i+2] == 0){
				array[i+2] = array[i];
				GraphAddRib (&my_graph, i+1, i+3, 0);
			}
			
        }
    }
    GraphPrint (&my_graph);
    GraphDeinit(&my_graph);
    
    return 0;
}

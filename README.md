# MSU_graph_C

The project implements struct Graph and a library with functions to work with the graph.

 ABOUT THE GRAPH 
 =====================

    struct Graph {
        char is_oriented;          
        int n_ribs; 
        int n_nodes;
        int** table;
    };

  The stucture containes the number of ribs, nodes and a dynamic array with pointers on
arrays 1x3 with information about each rib. The table of ribs represents the order number
of a start node, end node and rib's weight (which is 0 by default). 
  For example, if we have an unweighed graph:
  
  2-3      5_    9   10
 / \       |_|   |  
1   4-8_         6 
   /  |_|
  7
  
  the table will look like:
1) 1|2|0
2) 2|3|0
3) 2|4|0
4) 4|8|0
5) 8|8|0
6) 5|5|0
7) 9|6|0
8) 4|7|0

  As you can see, the 10'th and 7'th nodes are not representated int the table, but the
presence of this node is provided by parameter n_nodes, which equals 10 and points on
existence of all nodes from №1 to №10.

 FUNCTIONS 
 =====================
 
  Library uses auxiliary libraries:
1) <stdio.h>
2) <stdlib.h>
3) <string.h>
4) <malloc.h>
5) <errno.h>
6) <limits.h>

  Notation:

[1] int GraphInit(struct Graph *);

[2] void GraphDeinit(struct Graph *);

[3] void GraphAddNode(struct Graph *);

[4] int GraphAddRib(struct Graph *, int, int, int);

[5] void GraphDeleteNode(struct Graph *, int);

[6] void GraphDeleteRib(struct Graph *, int);

[7] int GraphConnectedComp(struct Graph *, int*, char);

[8] long int GraphMinimal (struct Graph *, char);

[9] int GraphGetInfo (struct Graph *, char *);

### [1]  GraphInit
SYNOPSIS
    int GraphInit(struct Graph *graphptr);
    
DESCRIPTION
    The function creates a graph on which points graphptr, initializes number of nodes 
    and ribs by 0 and allocates memory for 10 lines in the table of ribs.
    
ERRORS
    Errors might occur while calling the malloc() function.
    
RETURN VALUE
    Returns 0 in case of success or errno in case of an error. Outputs the reason of an 
    error in stderr.
_________________________________________________________________________________________

### [2]  GraphDeinit

SYNOPSIS
    void GraphDeinit(struct Graph *graphptr);
    
DESCRIPTION
    The function frees all the memory wich was allocated for the graph on which point 
    graphptr.
_________________________________________________________________________________________

### [3]  GraphAddNode
  
SYNOPSIS
    void GraphAddNode(struct Graph *graphptr);
    
DESCRIPTION
    The function simply increases the number of nodes parameter in the given struct graph,
    on which point graphptr.
    
ERRORS
    Ann error might occur when the number of nodes before adding equals INT_MAX. In this
    case function outputs diagnosis in the stderr and quits.
_________________________________________________________________________________________

### [4]  GraphAddRib
  
SYNOPSIS
    int GraphAddRib(struct Graph *graphptr, int start_node, int end_node, int weight);
    
DESCRIPTION
    The function increases the number of ribs parameter in the given struct Graph on 
    which points graphptr and appends the table of ribs with the information about the 
    new rib: order number of the start node, end node and the weight. Also checks if 
    those order numbers exceed the number of ribs declared in the struct. 
    
    If the number of ribs is divisible by 10, the function reallocates memory which was 
    given to the array of pointers on the table of ribs, increasing it by 
    10*sizeof(int*). Then the function allocates memory for the new lines in the table.

ERRORS
    An error might occur when the number of ribs before adding equals INT_MAX. In this
    case function outputs diagnosis in the stderr and quits.
    
    An error might occur while calling the malloc() function.

RETURN VALUE
    Returns 0 in case of success, errno in case of an error or -1 in case of trying to 
    increase INT_MAX number of ribs.
_________________________________________________________________________________________

### [5]  GraphDeleteNode

SYNOPSIS
    void GraphDeleteNode(struct Graph *graphptr, int node_number);
    
DESCRIPTION
    The function deletes a rib with the given order number node_number from the graph on
    which points graphptr. It also: 
    - deletes all the ribs (by GraphDeleteRib()) which are connected to the node that is 
    about to be deleted;
    - decreases by 1 all the order number of nodes in the table of ribs in case they 
    exceed the order number of node that is about to be deleted;
    - decreases by 1 the n_ribs parameter in the original graph.

ERRORS
    An error might occur when the number of nodes equals 0. In this case function outputs
    diagnosis in the stderr and quits.
    
EXAMPLE
    if we have an unweighed struct Graph my_graph:
  
      2-3      5_    9   10
     / \       |_|   |  
    1   4-8_         6 
       /  |_|
      7
      
    the table will look like:
    1) 1|2|0
    2) 2|3|0
    3) 2|4|0
    4) 4|8|0
    5) 8|8|0
    6) 5|5|0
    7) 9|6|0
    8) 4|7|0
    
    After calling GraphDeleteNode(&my_graph, 4) the 4th node and all connected ribs will
    be deleted (№3, №4 and №8), the table will look like:
    1) 1|2|0
    2) 2|3|0
    3) 7|7|0
    4) 4|4|0
    5) 8|5|0
    
    and the graph will look like:   
      
      2-3      4_    8   9
     /         |_|   |  
    1     7_         5    
       6  |_|    
_________________________________________________________________________________________

### [6]  GraphDeleteRib

SYNOPSIS
    void GraphDeleteRib(struct Graph *graphptr, int rib_number);
    
DESCRIPTION
    The function deletes a rib with the given order number rib_number from the graph on
    which points graphptr: removes this rib from the table of ribs and shifts the whole 
    table. 
    
    It also checks if the number of ribs is divisible by 10. If so, the function frees 
    the memory allocated for 10 lines in the table and reallocates memory.

ERRORS
    An error might occur when the number of ribs equals 0. In this case function outputs
    diagnosis in the stderr and quits.
_________________________________________________________________________________________


### [7]  GraphConnectedComp
  
SYNOPSIS
    int GraphConnectedComp(struct Graph *graphptr, int *nodes_groups, char param);

DESCRIPTION
    Connected components is either a separate node or a group of bonded nodes. 
    
    The function fills an array of integer group numbers (index corresponds to the order
    number of a node, value is a group number), goes through the table of ribs 
    and separates all nodes to groups according to conditions:
    1) If the start and the end node are met for the firt time, they are given the unique
    group number and marked as a new component (the same number in the array of flags).
    2) Only one of the nodes (either start or end) is met for the first time. So it is 
    given the same group number as the already met node.
    3) Both nodes were met, but they marked as members of different components. So all 
    the members of one components get the number of another components, and those group 
    becomes a one united component.
    
    If an element in the array temp[i] == 0 this means that the node №i+1 is not 
    connected with any other node, so it is a separate component. If elements have the 
    same value this means nodes are in the same group of connected components. 
    
    Outputs components if char = const DEBUG_MODE=1. Otherwise does not output anything.

ERRORS
    An error might occur when the number of nodes equals 0. In this case function outputs
    diagnosis in the stderr and quits.
    
RETURN VALUE
    Returns the number of connected components or -1 in case of an error.
    
EXAMPLE
    if we have an unweighed struct Graph my_graph:
    
      2-3      5_    9   10
     / \       |_|   |  
    1   4-8_         6 
       /  |_|
      7
    
    Nodes 1, 2, 3, 4, 7, 8 are in the 1st connected components, 6, 9 are in the 2nd.
    Nodes 5 and 10 are separate.
    Total number of connected components is 4.
    
    char array[10];
    
    After calling GraphDeleteNode(&my_graph, array, 0) the array will look like:
    1|1|1|1|0|2|1|1|2|0   // elements of array
    1 2 3 4 5 6 7 8 9 10  // index equals order number of a node
    
    The return value is 4.

_________________________________________________________________________________________

### [8]  GraphMinimal

SYNOPSIS
    long int GraphMinimal (struct Graph *graphptr, char param);

DESCRIPTION
    Makes the graph with the smallest weight by deleting ribs from the original graph.
    - Firstly counts and prints the weight of an original graph.
    - Works only with graphes with 1 connected component. The function checks this 
    condition using the function GraphConnectedComp(). 
    - Function usese Graphsort() to sort the graph by the weight of ribs.
    - According to the theorem given on the discreet math, the minimal quantity of ribs 
    in a one connected component equals n_nodes - 1. If this condition is met, the 
    minimal spanning graph equals original graph, the function returns the weight of an 
    original graph and stoppes.
    - If the number of nodes equals 1, it means that the graph consists of multiple 
    loops, so the function deletes all the ribs and stoppes.

    The function uses an array of integers with the number of elements that equals the 
    number of nodes. The index corresponds to one node's order number. 
    
    While going through the sorted table of ribs, the function automaticly deletes all 
    loops (by GraphDeleteRib()). Then it operates according to conditions (similar as in
    the function GraphConnectedComp()):
    1) If the start and the end node are met for the firt time, they are given the unique
    integer key and marked as a new component (the same number in the array of flags).
    2) Only one of the nodes (either start or end) is met for the first time. So it is 
    given the same key as the already met node.
    3) Both nodes were met, but they marked as members of different components. So all 
    the members of one components get the number of another components, and those group 
    becomes a one united component.
    4) Both nodes are from one component. So the current rib is deleted by 
    GraphDeleteRib().

ERRORS
    An error might occur when the number of nodes equals 0. In this case function outputs
    diagnosis in the stderr and quits.
    
    An error might occur when the number of connected components is more than 1. In this 
    case function outputs diagnosis if the param = DEBUG_MODE = 5 and quits.

RETURN VALUE
    Returns the weight of the new graph or -1 in case of an error.

EXAMPLE
    if we have an weighed struct Graph my_graph (weights are not shown on the picture):
    
      2-3   5_      
     / \   / \|    
    1   4-8__6      
    _  /  |_/
   |_|7
    
    the table will look like:
    
    1) 1|2|10
    2) 2|3|13
    3) 2|4|2
    4) 4|8|0
    5) 8|6|12
    6) 8|6|5
    7) 8|5|0
    8) 5|6|2
    9) 5|6|13
    10)7|4|1
    11)7|7|3
    
    THe old value is 10+13+2+0+12+5+0+2+13+1+3 = 61.
    
    After calling GraphMinimal(&my_graph, 0) ribs №11 (it is a loop) №5 and №9 will be 
    deleted. The table will look like:
    
    1) 1|2|10
    2) 2|3|13
    3) 2|4|2
    4) 4|8|0
    5) 8|6|5
    6) 8|5|0
    7) 5|6|2
    8) 7|4|1
        
    The graph will look like: 
        
      2-3   5      
     / \   / \   
    1   4-8__6      
       /  
      7
    
    The return value (new weight) is 10+13+2+0+5+0+2+1 = 33.
        
_________________________________________________________________________________________

### [9]  GraphGetInfo

SYNOPSIS
    void GraphGetInfo (struct Graph *graphptr, char *name_of_stream);

DESCRIPTION
    The function receives the pointer on the struct Graph and the name of stream from 
    which the data about graph will be received. It might be stdin (the information would
    be get from stdin) or name of any other file.
    
    It goes through the whole file in search for the first digit, then it reads number 
    of nodes by calling strtol(). If the number fits in integer it gives this value to
    graphptr->n_nodes. 
    
    Then it reads information line by line and fills the struct Graph by using 
    InputControl() function.
    
    GraphGetInfo() function can read an infinetly long string.
    
ERRORS
    An error might occur when the file does not contain any digit. So the function 
    outputs diagnose in stderr and quits with the value -1.
    
    An error might occur when the read number exceeds INT_MAX. So the function outputs 
    diagnose in stderr and quits with the value -1.
    
    An error might occur when the return value of fopen() function is NULL, which means 
    the file can not be opened. So the function outputs diagnose in stderr and quits with
    the errno value.
    
    An error might occur while allocating memory for the string. So the function outputs 
    diagnose in stderr and quits with the value errno.

RETURN VALUE
    Returns 0 in case of succsess, errno or -1 in case of an error.

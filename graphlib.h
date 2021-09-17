#define WAR1 "Warning: too few nodes. The number of nodes was increased" 
#define SORT_START "1" 
#define SORT_END "2" 
#define SORT_WEIGHT "3"
#define WORKING_MODE "5"
#define DEBUG_MODE "4"

struct Graph {
    int** table; //№ start node, № fin node, weight
    int n_ribs; 
    int n_nodes;  
};

void match_nodes (int, int *);
//  The function checks if the number of nodes (the first paramener) does not exceed the 
//number of nodes which has been declared in the struct Graph. If so, the number of nodes
//in the graph increases to the value given in the first parameter.
//------------------------------------------------------------------------------------

int GraphInit(struct Graph *);
//  The function creates a Graph, initializes orientation as "not oriented", number of
// nodes and ribs by 0 and allocates memory for 10 lines in the table of ribs.
// Indexation from '0'.
//------------------------------------------------------------------------------------

void GraphDeinit(struct Graph *);
//The function frees all the memory wich was allocated for the graph.
//------------------------------------------------------------------------------------

void GraphAddNode(struct Graph *);
//The function increases the number of nodes parameter in the given struct Graph.
//------------------------------------------------------------------------------------

int GraphAddRib(struct Graph *, int, int, int);
//  The function increases the number of ribs parameter in the given struct Graph and 
//appends the table of ribs with the information about the new rib: order number of the
//start node, end node and the weight. Checks if those order numbers exceed the number
//of ribs declared in the struct. 
//Returns 0 in case of success or -1 in case of an error.
//------------------------------------------------------------------------------------

int InputControl (char *, struct Graph *);
// Analyzes input line, filles start_node, end_node and weight with data.
// Returns 0 in case of success or -1 in case of an error.
// Outputs the reason of an error.
//------------------------------------------------------------------------------------

void GraphDeleteNode(struct Graph *, int);
//   The function deletes a rib with the given order number. It also: 
//deletes all the ribs (by GraphDeleteRib()) which are connected to the node that is 
//about to be deleted, decreases by 1 all the order number of nodes in the table 
//of ribs in case they exceed the order number of node that is about to be deleted;
//decreases by 1 the n_ribs parameter in the original tree.
//  Returns 0 in case of success or -1 in case of an error.
//------------------------------------------------------------------------------------

void GraphDeleteRib(struct Graph *, int);
//Function deletes 1 line from the table of ribs, shifts the table and frees memory.
//------------------------------------------------------------------------------------

void GraphPrint(struct Graph *);
//The function prints information about the graph's orientation, the number of ribs and
//nodes and the whole table of ribs.
//------------------------------------------------------------------------------------

void GraphSort(struct Graph *, char);
// Ascending sort of the table of ribs depending on the parameter. 
//------------------------------------------------------------------------------------

int GraphConnectedComp(struct Graph *, int*, char);
// The function fills an array of integer group numbers (index corresponds to the order
//number of a node, value is a group number), goes through the table of ribs and separates 
//all nodes 
//------------------------------------------------------------------------------------

long int GraphMinimal (struct Graph *, char);
//  Makes the graph with the smallest weight by deleting ribs from the original graph.
//  Returns the weight of the new graph or -1 in case of an error.
//------------------------------------------------------------------------------------

int GraphGetInfo (struct Graph *, char *);
//  Gets information about the graph from a file with name input_str (maybe stdin).
//  Fills the number of ribs, call InputControl. Can analyze a very big sting.

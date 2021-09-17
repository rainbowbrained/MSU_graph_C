#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <limits.h>

#define WAR1 "Warning: too few nodes. The number of nodes was increased" 
#define SORT_START "1" 
#define SORT_END "2" 
#define SORT_WEIGHT "3"
#define DEBUG_MODE "4"
#define WORKING_MODE "5"

//Info about graph
struct Graph {
    int** table; //№ start node, № fin node, weight
    int n_ribs; 
    int n_nodes;  
};

void match_nodes (int, int *);
int GraphInit(struct Graph *);
void GraphDeinit(struct Graph *);
void GraphAddNode(struct Graph *);
int GraphAddRib(struct Graph *, int, int, int);
int InputControl (char *, struct Graph *);
void GraphDeleteNode(struct Graph *, int);
void GraphDeleteRib(struct Graph *, int);
void GraphPrint(struct Graph *);
void GraphSort(struct Graph *, char);
int GraphConnectedComp(struct Graph *, int*, char);
long int GraphMinimal (struct Graph *, char);
int GraphGetInfo (struct Graph *, char *);


//------------------------------------------------------------------------------------
//  The function checks if the number of nodes (the first paramener) does not exceed the 
//number of nodes which has been declared in the struct Graph. If so, the number of nodes
//in the graph increases to the value given in the first parameter.
void match_nodes (int x, int *y)
{
    if (x > *y) {
        fprintf (stderr, "%s from %i to %i\n",WAR1, *y, x);
        *y = x;
    }
    return;
}

//------------------------------------------------------------------------------------
//  The function creates a Graph, initializes orientation as "not oriented", number of
// nodes and ribs by 0 and allocates memory for 10 lines in the table of ribs.
// Indexation from '0'.
int GraphInit(struct Graph *g) {
    g->table = (int **) malloc(10*sizeof(int*)); //10 lines 
    if (g->table == NULL) {
        perror("Error (while creating a graph): can not allocate memory.\n");
        return errno;
    }
    for (int i = 0; i < 10; i++) {
        g->table[i] = (int*) malloc(3*sizeof(int)); //3 elements
        if (g->table[i] == NULL) {
            perror("Error (while creating a graph): can not allocate memory.\n");
            free(g->table);
            return errno;
        }
        g->table[i][0] = 0;
        g->table[i][1] = 0;
        g->table[i][2] = 0;
    }   
    g->n_ribs = 0; 
    g->n_nodes = 0;
    return 0;
}

//------------------------------------------------------------------------------------
//The function frees all the memory wich was allocated for the graph.
void GraphDeinit(struct Graph *g) {
    int num = (g->n_ribs + 10) / 10;
    
    for (int i = 0; i < num*10; i++) {
        free (g->table[i]);
    }
    free(g->table);       
    g->n_ribs = 0; 
    g->n_nodes = 0;
    return;
}

//------------------------------------------------------------------------------------
//The function increases the number of nodes parameter in the given struct Graph.
void GraphAddNode(struct Graph *g) {
	
	if (g->n_nodes == INT_MAX) {
		fprintf(stderr, "Error: reached the maximum possible number of nodes.\n");
		return;
	}
	
    g->n_nodes++;
    return;
}

//------------------------------------------------------------------------------------
//  The function increases the number of ribs parameter in the given struct Graph and 
//appends the table of ribs with the information about the new rib: order number of the
//start node, end node and the weight. Checks if those order numbers exceed the number
//of ribs declared in the struct. 
//Returns 0 in case of success or -1 in case of an error.
int GraphAddRib(struct Graph *g, int start, int fin, int weight) {
	if (g->n_ribs == INT_MAX) {
		fprintf(stderr, "Error: reached the maximum possible number of ribs.\n");
		return -1;
	}
	
	if ((start <= 0)||(fin <= 0)) {
		fprintf(stderr, "Error: order number of start and end node");
		fprintf(stderr, "must be positive integer.\n");
		return -1;
	}
	
    match_nodes (start, &(g->n_nodes));
    match_nodes (fin, &(g->n_nodes));
    
    if ((g->n_ribs % 10) == 9) {
		
            g->table = (int**)realloc(g->table, (g->n_ribs + 11)*sizeof(int*));
            if (g->table == NULL) {
                perror("Error (while adding a rib): can not allocate memory.\n");
                return errno;
            }
            
            for (int i = 0; i < 10; i++) {
                g->table[g->n_ribs + i + 1] = (int*) malloc(3*sizeof(int));
                if (g->table[g->n_ribs + i + 1] == NULL) {
                    perror("Error (while adding a rib): can not allocate memory.\n");
                    return errno;
                }
            }
        }
    g->n_ribs++;
    g->table[g->n_ribs - 1][0] = start;
    g->table[g->n_ribs - 1][1] = fin;
    g->table[g->n_ribs - 1][2] = weight;
    return 0;
}

//------------------------------------------------------------------------------------
// Analyzes input line, filles start_node, end_node and weight with data.
// Returns 0 in case of success or -1 in case of an error.
// Outputs the reason of an error.
int InputControl (char *input_str, struct Graph *g) {
    char c;
    long int temp = 0;
    int info[3] = {0,0,0}; //start, fin, end
    
    for (int i = 0; i < strlen(input_str); i++) {
        c = input_str[i];
        if ((c!=' ')&&((c<'0')||(c>'9'))) {
            fprintf (stderr, "Error: wrong input in the string %i.\n", g->n_ribs + 1); 
            return -1;
        } 
        
        if ((c == ' ')||(i == strlen(input_str) - 1)) { //came to SPACE or end of string
            if ((c == ' ')&&(info[1] != 0)&&(temp != 0)) {
                info[2] = (int)temp;
                goto InputControl_ending;
            }
            
            if (c != ' ') {
                temp = temp*10 + c - '0';
                if (temp > INT_MAX) {
					fprintf(stderr,"Error: number in the string %i", g->n_ribs + 1);
					fprintf(stderr,"exceeds max integer.\n");
					return -1;
				}	
            }
            
            if (info[0] == 0) {
                match_nodes(temp, &(g->n_nodes));
                info[0] = (int)temp;
                temp = 0;
            } else if (info[1] == 0) {
                match_nodes(temp, &(g->n_nodes));
                info[1] = (int)temp;
                temp = 0;
            } else {
                info[2] = (int)temp;
                temp = 0;
            }
        }  else {
            temp = temp*10 + c - '0';
            if (temp > INT_MAX) {
			    fprintf(stderr,"Error: number in the string %i exceeds max integer.\n", g->n_ribs + 1);
				return -1;
			}	
        }
    }
    
    if (info[1] == 0) {
        fprintf (stderr, "Error: undefined rib № %i\n", g->n_ribs + 1); //undefined start and/or ending of a rib
        return -1;
    } 
InputControl_ending:
    GraphAddRib (g, info[0], info[1], info [2]);
    return 0;
}

//------------------------------------------------------------------------------------
//   The function deletes a rib with the given order number. It also: 
//deletes all the ribs (by GraphDeleteRib()) which are connected to the node that is 
//about to be deleted, decreases by 1 all the order number of nodes in the table 
//of ribs in case they exceed the order number of node that is about to be deleted;
//decreases by 1 the n_ribs parameter in the original tree.
//  Returns 0 in case of success or -1 in case of an error.
void GraphDeleteNode(struct Graph *g, int number) {
	if (g->n_nodes == 0) {
	    fprintf (stderr, "Error: trying to delete a node in an empty graph.\n");
	    return;
	}
    for (int i = g->n_ribs - 1; i >= 0; i--) {
        if ((g->table[i][0] == number)||(g->table[i][1] == number)) {
            GraphDeleteRib(g, i+1); //deleting the rib connected to a deleting node
        } else {
            if (g->table[i][0] > number) {
                g->table[i][0]--;
            }
            if (g->table[i][1] > number) {
                g->table[i][1]--;
            }//decreasing the order number
        }
    }
    g->n_nodes--;
    return;
}

//------------------------------------------------------------------------------------
//Function deletes 1 line from the table of ribs, shifts the table and frees memory.
void GraphDeleteRib(struct Graph *g, int number) { 
	if (g->n_ribs == 0) {
	    fprintf (stderr, "Error: trying to delete a rib in a graph without ribs.\n");
	    return;
	}
    for (int i = number - 1; i< g->n_ribs - 1; i++) {
        g->table[i][0] = g->table[i + 1][0];
        g->table[i][1] = g->table[i + 1][1];
        g->table[i][2] = g->table[i + 1][2];
    }
    if ((g->n_ribs % 10) == 0) {
            for (int i = 0; i < 10; i++) {
            free(g->table[g->n_ribs + i]);
            }
        g->table = (int**)realloc(g->table, (g->n_ribs)*sizeof(int*));
    }
    g->n_ribs--;
    return;
}

//------------------------------------------------------------------------------------
//The function prints information about the graph's orientation, the number of ribs and
//nodes and the whole table of ribs.
void GraphPrint(struct Graph *g) {
    printf("The number of nodes is %i\n", g->n_nodes);
    printf("The number of ribs is %i\nThe list of ribs:\n", g->n_ribs);
    printf("№ of a rib) № of a start node|№ of an end node|Weight of a rib\n");
    
    for (int i = 0; i < g->n_ribs; i++) {
        printf("%i) %i|%i|%i\n", i+1, g->table[i][0], g->table[i][1], g->table[i][2]);
    }
    return;
}

//------------------------------------------------------------------------------------
// Ascending sort of the table of ribs depending on the parameter. 
void GraphSort(struct Graph *g, char param) {
	if (g->n_nodes == 0) {
		fprintf (stderr, "Warning: trying to sort an empty graph.\n");
		return;
	}
    int j, i, n = g->n_ribs;
    int * temp;
    if (param == 1) {
        for (i = 1; i < n; i++) {
            for (j = 0; j < n - i; j++) {
                if (g->table[j][0] > g->table[j + 1][0]) {
                    temp = g->table[j];
                    g->table[j] = g->table[j + 1];
                    g->table[j + 1] = temp;
                }
            }
        }
    } else if (param == 2) {
        for (i = 1; i < n; i++) {
            for (j = 0; j < n - i; j++) {
                if (g->table[j][1] > g->table[j + 1][1]) {
                    temp = g->table[j];
                    g->table[j] = g->table[j + 1];
                    g->table[j + 1] = temp;
                }
            }
        }
    } else {
        for (i = 1; i < n; i++) {
            for (j = 0; j < n - i; j++) {
                if (g->table[j][2] > g->table[j + 1][2]) {
                    temp = g->table[j];
                    g->table[j] = g->table[j + 1];
                    g->table[j + 1] = temp;
                }
            }
        }
    }
    return;
}

//------------------------------------------------------------------------------------
// The function fills an array of integer group numbers (index corresponds to the order
//number of a node, value is a group number), goes through the table of ribs and separates
//all nodes 
int GraphConnectedComp(struct Graph *g, int* temp, char param) {
	if (g->n_nodes == 0) {
		fprintf (stderr, "Warning: can not find connected components in empty graph.\n");
		return -1;
	}
    int start, fin, k, j = 1, i;
    char flag = 0;
    
    for (i = 0; i < g->n_nodes; i++) {
        temp[i] = 0;
    }
    
    for (i = 0; i < g->n_ribs; i++) {
        start = g->table[i][0] - 1;
        fin = g->table[i][1] - 1;
        if (start == fin) {
            //this is a loop
            continue; 
        }
        if ((temp[start] == 0)&&(temp[fin] == 0)) {
            temp[start] = j; //j - unique group number
            temp[fin] = j; 
            j++;
        } else if ((temp[start] != 0)&&(temp[fin] == 0)) {
            temp[fin] = temp[start];
        } else if ((temp[start] == 0)&&(temp[fin] != 0)) {
            temp[start] = temp[fin];
        } else if (temp[start] != temp[fin]) { 
            //change the number of group of connected nodes
            int tmp1 = temp[fin]; 
            int tmp2 = temp[start];
            
            for (k = 0; k < g->n_nodes; k++) {
                if (temp[k] == tmp1) {
                    temp[k] = tmp2;
                }
            }
        }
    }
    
    flag = 0;
    start = 0; //counter of components
    fin = 0;
    for (i = 0; i < j; i++) { //cycle by group numbers
        for (k = 0; k < g->n_nodes; k++){ //cycle by node numbers
            if ((temp[k] == 0)&&(i == 0)) {
                if (flag == 0) {
                    if (param == '4') {
                        printf ("Separate nodes: ");
                    }
                    flag++;
                }
                if (param == '4') {
                    printf ("%i ", k + 1);
                }
                start++;
            } else if (temp[k] == i) {
                if (flag == 0) {
                    if (param == '4') {
                        printf ("\nGroup of connected components №%i: ", fin + 1);
                    }
                    fin++;
                    flag++;
                    start++;
                }
                if (param == '4') {
                    printf("%i ", k + 1);
                }
            }
        }
        flag = 0;
    }
    if (param == '4') {
        printf("\n");
    }
    return start;
}

//------------------------------------------------------------------------------------
//  Makes the graph with the smallest weight by deleting ribs from the original graph.
//  Returns the weight of the new graph or -1 in case of an error.
long int GraphMinimal (struct Graph *g, char param) {
	if (g->n_nodes == 0) {
		fprintf (stderr, "Warning: trying to find minimal graph in am empty graph.\n");
		return -1;
	}
	
    int array[g->n_nodes];
    int concomp = GraphConnectedComp(g, array, 2);
    if (concomp != 1) { //Now is sorted
        if (param == 5) {
            printf ("The number of connected components is %i - more than 1.\n", concomp);
            printf ("Can not find a minimal spanning tree.\n");
        }
        return -1;
    }
    int i, k = 0, j = 0; 
    long int weight = 0;
    for (i = 0; i < g->n_ribs; i++) {
        weight += g->table[i][2];
    }
    if (param == 5) {
        printf ("The weight of original graph is %li\n", weight);
    }
    if (g->n_ribs == g->n_nodes - 1) {
        if (param == 5) {
            printf ("Minimal spanning graph equals original graph.\n");
        }
        return weight;
    }
    GraphSort (g, 3); //made for speeding up the work of this function
    
    if (g->n_nodes == 1) { //graph consists of 1 node, so all ribs are loops
        for (i = g->n_ribs; i > 0; i--) {
            GraphDeleteRib(g, i);
        }
        return 0;
    }
    
    int flag[g->n_nodes]; //flag[i] = 1 if node №(i+1) is included in minimal graph
    for (i = 0; i < g->n_nodes; i++) {
        flag[i] = 0;
    }
    weight = 0;
    for (i = 0; i < g->n_ribs; i++) {
        if (g->table[i][0] == g->table[i][1]) {
            // It is a loop
            GraphDeleteRib(g, i+1);
            i--;
            continue;
        }
        if ((flag[g->table[i][0] - 1] == 0)&&(flag[g->table[i][1] - 1] == 0)) {
            k++;
            flag[g->table[i][0] - 1] = k; //k - unique group number
            flag[g->table[i][1] - 1] = k;
            weight += g->table[i][2];
            j++;
        } else if ((flag[g->table[i][0] - 1] == 0)&&(flag[g->table[i][1] - 1] != 0)) {
            flag[g->table[i][0] - 1] = flag[g->table[i][1] - 1];
            weight += g->table[i][2];
            j++;
        } else if ((flag[g->table[i][0] - 1] != 0)&&(flag[g->table[i][1] - 1] == 0)) {
            flag[g->table[i][1] - 1] = flag[g->table[i][0] - 1];
            weight += g->table[i][2];
            j++;
        } else if (flag[g->table[i][0] - 1] != flag[g->table[i][1] - 1]) {
            weight += g->table[i][2];
            j++;
            //change the number of group of connected nodes
            int tmp1 = flag[g->table[i][0] - 1]; 
            int tmp2 = flag[g->table[i][1] - 1];
            
            for (int m = 0; m < g->n_nodes; m++) {
                if (flag[m] == tmp1) {
                    flag[m] = tmp2;
                }
            }
        } else {
            GraphDeleteRib(g, i+1);
            i--;
        }
    }
    if (param == 5) {
        printf ("The weight of the minimal spanning graph %li\n", weight);
    }
    return weight;
}

//------------------------------------------------------------------------------------
//  Gets information about the graph from a file with name input_str (maybe stdin).
//  Fills the number of ribs, call InputControl. Can analyze a very big sting.
int GraphGetInfo (struct Graph *g, char *input_str) {
    char *str = malloc(100*sizeof(char));
    
    if (str == NULL) {
        perror("Error (while getting information): can not allocate memory.\n");
        return errno;
    }
    
    char c, flag = 1;
    int len = 1;
    
    if (!strcmp(input_str,"stdin")) {
        //Searching for the first digit
        while(((c = fgetc(stdin)) < '0') || (c > '9')) {
            if (c == EOF) {
                fprintf(stderr, "Error. Undeclaired number of ribs.\n");
                free(str);
                return -1;
            }
        }
        str[len - 1] = c;
        len++;
        
        //Get the number of nodes
        while(((c = fgetc(stdin)) != '\n') && (c != EOF)) {
            str[len - 1] = c;
            len++;
            if (len % 100 == 0) {
                str = realloc(str, len + 100);
                if (str == NULL) {
                    perror("Error: can not allocate memory.\n");
                    return errno;
                }
            }
            str[len - 1] = '\0';
        }
        if (strtol(str, NULL, 10) >= INT_MAX) {
			fprintf(stderr, "Error: impossible value for the number of nodes.\n");
			g->n_nodes = 0;
			free(str);
			return -1;
		}
		g->n_nodes = (int)strtol(str, NULL, 10);
        
        if (len > 100) {
            str = realloc(str, 100);
        }
        len = 1;
        
        //Get information about ribs
        while (flag) {
			//Searching for the first digit
            while(((c = fgetc(stdin)) < '0') || (c > '9')) {
                if (c == EOF) {
                    free(str);
                    return -1;
                }
            }
            str[0] = c;
            len++;
			
            while(((c = fgetc(stdin)) != '\n') && (c != EOF)) {
                str[len - 1] = c;
                len++;
                if (len % 100 == 0) {
                    str = realloc(str, len + 100);
                    if (str == NULL) {
                        perror("Error: can not allocate memory.\n");
                        return errno;
                    }
                }
                str[len - 1] = '\0';
            }
            if (c == EOF) flag = 0;
            
            InputControl (str, g);
            if (len > 100) {
                str = realloc(str, 100);
            }
            for (int i = 0; i < 100; i++) str[i] = ' ';
            len = 1;
        }
        
        
    } else {
        FILE* f = fopen (input_str, "r");
        
        if (f == NULL) {
			fprintf(stderr, "Error. Can not open the file.\n");
            free(str);
            return errno;
		}	
        
        while(((c = fgetc(f)) < '0') || (c > '9')) {
            if (c == EOF) {
                fprintf(stderr, "Error. Undeclaired number of ribs.\n");
                free(str);
                return -1;
            }
        }
        str[len - 1] = c;
        len++;
        
        //Get the number of nodes
        while(((c = fgetc(f)) != '\n') && (c != EOF)) {
            str[len - 1] = c;
            len++;
            if (len % 100 == 0) {
                str = realloc(str, len + 100);
            }
            str[len - 1] = '\0';
        }
        
        if (strtol(str, NULL, 10) >= INT_MAX) {
			fprintf(stderr, "Error: impossible value for the number of nodes.\n");
			g->n_nodes = 0;
			free(str);
			return -1;
		}
		g->n_nodes = (int)strtol(str, NULL, 10);
        
        if (len > 100) {
            str = realloc(str, 100);
        }
        len = 1;
        
        //Get information about ribs
        while (flag) {
			//Searching for the first digit
			
            while(((c = fgetc(f)) < '0') || (c > '9')) {
                if (c == EOF) {
                    free(str);
                    return -1;
                }
            }
            str[0] = c;
            len++;
			
            while (((c = fgetc(f)) != '\n') && (c != EOF)) {
                str[len - 1] = c;
                len++;
                if (len % 100 == 0) {
                    str = realloc(str, len + 100);
                }
                str[len - 1] = '\0';
            }
            if (c == EOF) flag = 0;
            
            InputControl (str, g);
            if (len > 100) {
                str = realloc(str, 100);
            }
            for (int i = 0; i < 100; i++) str[i] = ' ';
            len = 1;
        }
        
        fclose (f);
     }
    free (str);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>




// Data structure to store nodes of the graph
typedef struct Node {
    int id;
} Node;

// Data structure to store a graph edge
typedef struct Edge {
    int src;
    int dest;
    int weight;
} Edge;

// Data structure to store a graph object
typedef struct Graph {
    int numOfNodes;
    int numOfEdges;
    struct Node *nodes;
    struct Edge *edges;
} Graph;

int initGraph(struct Graph *g, char mainInp) {
    int numOfNodes;
    scanf(" %d", &numOfNodes);
    Node *nodes = (Node *) malloc(numOfNodes * sizeof(Node));
    if (nodes == NULL) return 0;
    int nodeInd = 0;
    Edge *edges = (Edge *) malloc(numOfNodes * (numOfNodes - 1) * sizeof(Edge));
    if (edges == NULL) return 0;
    int edgeInd = 0;
    scanf(" %c", &mainInp);
    while (mainInp == 'n') {
        int nodeID;
        scanf(" %d", &nodeID);
        Node n1;
        n1.id = nodeID;
        nodes[nodeInd] = n1;
        nodeInd++;

        int currDest;
        int currWeight;
        while (scanf(" %d %d", &currDest, &currWeight) == 2) {
            struct Edge e1;
            e1.src = nodeID;
            e1.dest = currDest;
            e1.weight = currWeight;
            edges[edgeInd] = e1;
            edgeInd++;
        }
        scanf(" %c", &mainInp);
    }
    struct Edge *newEdges = (struct Edge *) malloc(edgeInd * sizeof(struct Edge));
    for (int i = 0; i < edgeInd; i++) //shrinking array to actual amount of edges
    {
        struct Edge e1;
        e1.src = edges[i].src;
        e1.dest = edges[i].dest;
        e1.weight = edges[i].weight;
        newEdges[i] = e1;
    }
    free(edges);

    g->numOfNodes = numOfNodes;
    g->numOfEdges = edgeInd;
    g->nodes = nodes;
    g->edges = newEdges;
    return mainInp;
}

int min(int a, int b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

int *FloydWarshallInit(struct Graph *g) {
    int *arr = (int *) malloc((g->numOfNodes * g->numOfNodes) * sizeof(int));
    for (int i = 0; i < g->numOfNodes; ++i) {
        for (int j = 0; j < g->numOfNodes; ++j) {
            if (i == j) {
                arr[(i * g->numOfNodes) + j] = 0;
            } else {
                for (int k = 0; k < g->numOfEdges; ++k) {
                    if ((g->edges[k].src == g->nodes[i].id) && (g->edges[k].dest == g->nodes[j].id)) {
                        arr[(i * g->numOfNodes) + j] = g->edges[k].weight;
                        break;
                    } else {
                        arr[(i * g->numOfNodes) + j] = 100000;
                    }
                }
            }
        }
    }
    for (int k = 0; k < g->numOfNodes; k++) {
        for (int i = 0; i < g->numOfNodes; i++) {
            for (int j = 0; j < g->numOfNodes; j++) {
                if (i != j) {
                    if (arr[(i * g->numOfNodes) + k] != 0 && arr[(k * g->numOfNodes) + j] != 0 &&
                        arr[(i * g->numOfNodes) + j] == 0) {
                        arr[(i * g->numOfNodes) + j] = arr[(i * g->numOfNodes) + k] + arr[(k * g->numOfNodes) + j];
                    }
                    if (arr[(i * g->numOfNodes) + k] != 0 && arr[(k * g->numOfNodes) + j] != 0 &&
                        arr[(i * g->numOfNodes) + j] != 0) {
                        int a = arr[(i * g->numOfNodes) + j];
                        int b = arr[(i * g->numOfNodes) + k];
                        int c = arr[(k * g->numOfNodes) + j];
                        arr[(i * g->numOfNodes) + j] = min(a, b + c);
                    }
                }
            }
        }
    }
    return arr;
}

//function to swap the variables
void swap(int *a, int *b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

int shortPath(Graph *g, int a, int b, int *floyd) {
    int src;
    int dst;
    for (int i = 0; i < g->numOfNodes; i++) {
        if (g->nodes[i].id == a) src = i;
        if (g->nodes[i].id == b) dst = i;
    }
    return floyd[(src * g->numOfNodes) + dst];

}

//permutation function
void permutation(int *arr, int start, int end, int *floyd, int *minVal, int size, Graph *g) {
    //adapted from: https://www.codesdope.com/blog/article/generating-permutations-of-all-elements-of-an-arra/
    if (start == end) {
        int tempVal = 0;
        for (int i = 0; i < size - 1; i++) {
            tempVal += shortPath(g, arr[i], arr[i + 1], floyd);
        }
        if (tempVal < *minVal) {
            *minVal = tempVal;
        }
        return;
    }
    int i;
    for (i = start; i <= end; i++) {
        //swapping numbers
        swap((arr + i), (arr + start));
        //fixing one first digit
        //and calling permutation on
        //the rest of the digits
        permutation(arr, start + 1, end, floyd, minVal, size, g);
        swap((arr + i), (arr + start));
    }
}

int main() {
    struct Graph g;
    int countA = 0;
    char mainInp;
    scanf("%c", &mainInp);
    int mainCount = 0;
    while (1) {
        if (mainInp == 'A') {
            mainCount = 0;
            countA++;
            if (countA >= 2) { //If graph already exists
                free(g.nodes);
                free(g.edges);
            }
            mainInp = initGraph(&g, mainInp);
            printf("A finished");
        }
        if (mainInp == 'B') {
            mainCount = 0;
            int nodeID;
            scanf(" %d", &nodeID);
            int countDelEdges = 0;
            int flag = 0;
            for (int i = 0; i < g.numOfNodes; i++) { //checks if node exists, finishes loop if exists
                if (g.nodes[i].id == nodeID) { //finding if new node exists in graph
                    flag = 1;
                    for (int j = 0; j < g.numOfEdges; j++) {
                        if (g.edges[i].src == nodeID) {
                            countDelEdges++;
                            g.edges[i].src = -1;
                        }
                    }
                    struct Edge *edges = (struct Edge *) malloc(
                            (g.numOfEdges - countDelEdges) * sizeof(struct Edge));
                    int currEdgeInd = 0;
                    for (int j = 0; j < g.numOfEdges; j++) {
                        if (g.edges[i].src != -1) {
                            struct Edge e1;
                            e1.src = g.edges[i].src;
                            e1.dest = g.edges[i].dest;
                            e1.weight = g.edges[i].weight;
                            edges[currEdgeInd] = e1;
                            currEdgeInd++;
                        }
                    }
                    free(g.edges);
                    g.edges = edges;
                    break;
                }
            }

            if (flag != 1) { // node doesnt exist in graph
                struct Node n1;
                n1.id = nodeID;
                g.numOfNodes++;
                struct Node *nodes = (struct Node *) realloc(g.nodes, g.numOfNodes * sizeof(struct Node));
                nodes[g.numOfNodes - 1] = n1;
                g.nodes = nodes;
            }
            struct Edge *edges = (struct Edge *) malloc(
                    g.numOfNodes * sizeof(struct Edge)); //init edges to max possible size
            int edgeInd = 0;
            int currDest;
            int currWeight;
            while (scanf(" %d %d", &currDest, &currWeight) == 2) {
                struct Edge e1;
                e1.src = nodeID;
                e1.dest = currDest;
                e1.weight = currWeight;
                edges[edgeInd] = e1;
                edgeInd++;
            }
            struct Edge *newEdges = (struct Edge *) malloc(edgeInd * sizeof(struct Edge));
            for (int j = 0; j < edgeInd; j++) //shrinking edges array to appropriate size
            {
                struct Edge e1;
                e1.src = edges[j].src;
                e1.dest = edges[j].dest;
                e1.weight = edges[j].weight;
                newEdges[j] = e1;
            }
            free(edges);

            struct Edge *edgesTotal = (struct Edge *) realloc(g.edges, (g.numOfEdges + edgeInd) * sizeof(struct Edge));
            for (int i = g.numOfEdges; i < g.numOfEdges + edgeInd; i++) {
                edgesTotal[i] = newEdges[i - g.numOfEdges];
            }
            g.edges = edgesTotal;
            g.numOfEdges += edgeInd;
            scanf(" %c", &mainInp);
            printf("B finished");
        }
        if (mainInp == 'D') {
            mainCount = 0;
            int nodeID;
            scanf(" %d", &nodeID);
            struct Node *nodes = (struct Node *) malloc((g.numOfNodes - 1) * sizeof(struct Node));
            int currNodeInd = 0;

            for (int i = 0; i < g.numOfNodes; i++)
                //remove nodes and put them in a new array (malloc used, need to free previous array from memory)
            {
                if (g.nodes[i].id != nodeID) {
                    struct Node n1;
                    n1.id = g.nodes[i].id;
                    nodes[currNodeInd] = n1;
                }
            }
            free(g.nodes);
            g.nodes = nodes;
            //Finished removing node.

            //removing edges
            struct Edge *newEdges = (struct Edge *) malloc(g.numOfEdges * sizeof(struct Edge));
            int currEdgeInd = 0;
            for (int i = 0; i < g.numOfEdges; i++) {
                if (g.edges[i].src != nodeID && g.edges[i].dest != nodeID) {
                    struct Edge e1;
                    e1.src = g.edges[i].src;
                    e1.dest = g.edges[i].dest;
                    e1.weight = g.edges[i].weight;
                    newEdges[currEdgeInd] = e1;
                }
            }
            free(g.edges);
            g.edges = newEdges;
            scanf(" %c", &mainInp);
            printf("D finished");
        }
        if (mainInp == 'S') {
            mainCount = 0;
            int src;
            int dest;
            scanf(" %d %d", &src, &dest);
            int *arr = FloydWarshallInit(&g);
            int length = shortPath(&g, src, dest, arr);
            free(arr);
            if (length == 10000) {
                length = -1;
            }
            printf("Dijsktra shortest path: %d \n", length);
            scanf(" %c", &mainInp);
        }
        if (mainInp == 'T') {
            mainCount = 0;
            int numOfTSPNodes;
            scanf(" %d", &numOfTSPNodes);

            int *tspNodes = (int *) malloc(numOfTSPNodes * sizeof(int));
            for (int i = 0; i < numOfTSPNodes; i++) {
                scanf(" %d", &(tspNodes[i]));
            }

            int *floyd = FloydWarshallInit(&g);
            int minPath = 100000;
            permutation(tspNodes, 0, numOfTSPNodes - 1, floyd, &minPath, numOfTSPNodes, &g);
            if (minPath == 100000) {
                minPath = -1;
            }
            printf("TSP shortest path: %d \n", minPath);
            scanf(" %c", &mainInp);
        } else {
            mainCount++;
            if (mainCount > 1) {
                break;
            }
        }
        
    }
}

#ifndef GRAPH_JRB_H
#define GRAPH_JRB_H

#include "../libfdr/dllist.h"
#include "../libfdr/fields.h"
#include "../libfdr/jval.h"
#include "../libfdr/jrb.h"

#define true 1
#define false 0
#define UNDIRECTED 0
#define DIRECTED 1
#define INT_MAX 2147483647
#define INFINITIVE_VALUE 10000000

typedef struct
{
    JRB edges;
    JRB vertices;
} Graph;

Graph createGraph();

void addVertex(Graph graph, int id, char *name);
void *getVertex(Graph graph, int id);
int getNumofV(Graph graph);

void addEdge(Graph graph, int v1, int v2, double weight);
double getEdgeValue(Graph graph, int v1, int v2);
int hasEdge(Graph graph, int v1, int v2);

int indegree(Graph graph, int v, int *output);
int outdegree(Graph graph, int v, int *output);

int DAG(Graph graph);
int DFS_DAG(Graph graph, int start);

int topologicalSort(Graph graph, int *output);

int DFS(Graph graph, int start, int stop, int *path);
//BFS
int shortest_noWeight_path(Graph graph, int start, int stop, int *path);

//Dijisktra
double shortestPath(Graph graph, int start, int stop, int *path, int *numVertices); 

void dropGraph(Graph graph);

#endif /* GRAPH_JRB_H */

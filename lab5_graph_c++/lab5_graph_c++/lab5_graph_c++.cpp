// lab5_graph_c++.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <iostream>
#include <ctime>
#include <vector>
const int inf = 1;
using namespace std;

struct listnode
{
	char *data;
	int value;
	struct listnode *next;
};
struct listnode *list_createnode( int value)
{
	struct listnode *p;
	p =(listnode*) malloc(sizeof(*p));
	if (p!= NULL)
	{
		//p->data = data;
		p->value = value;
		p->next = NULL;
	}
	return p;
}
struct queue1
{
	struct listnode *head;
	struct listnode *tail;
	int size;
};
struct queue1 *queue_create()
{
	struct queue1 *q = (queue1*)malloc(sizeof(*q));
	if (q != NULL)
	{
		q->size = 0;
		q->head = NULL;
		q->tail = NULL;
	}
	return q;
}
void queue_enqueue(struct queue1 *q, int value)
{
	struct listnode *oldtail = q->tail;
	q->tail = list_createnode(value);
	if (q->head == NULL)
	{
		q->head = q->tail;
	}
	else
	{
		oldtail->next = q->tail;
	}
	q->size++;
}
int queue_dequeue(struct queue1 *q)
{
	int value;
	struct listnode *p;
	if (q->size == 0)
	{
		return -1;
	}
	value = q->head->value;
	p = q->head->next;
	free(q->head);
	q->head = p;
	q->size--;
	return value;
}
void queue_free(struct queue1 *q)
{
	while (q->size>0)
	{
		queue_dequeue(q);
		
	}
	free(q);
}


int queue_size(struct queue1 *q)
{
	return q->size;
}
struct graph
{
	int nvertices;
	int *matrix;
	int *visited;
};
void graph_clear(struct graph *g)
{
	int i, j;
	for (i = 0; i < g->nvertices; i++)
	{
		g->visited[i] = 0;
		for (j = 0; j < g->nvertices; j++)
		{
			g->matrix[i*g->nvertices + j] = 0;
		}
	}
}
struct graph *graph_create(int nvertices)
{
	struct graph *g;
	g = (graph*)malloc(sizeof(*g));

	g->nvertices = nvertices;
	g->visited = (int*)malloc(sizeof(int)*nvertices);
	g->matrix = (int*)malloc(sizeof(int)*nvertices*nvertices);
	graph_clear(g);
	return g;
}

void graph_free(struct graph *g)
{
	free(g->matrix);
	free(g);
}
void graph_set_edge(struct graph *g, int i, int j, int w)
{
	g->matrix[(i - 1)*g->nvertices + j - 1] = w;
	g->matrix[(j - 1)*g->nvertices + i - 1] = w;
}
int graph_get_edge(struct graph *g, int i, int j)
{
	return g->matrix[(i - 1)*g->nvertices + j - 1];
}
void graph_dfs(struct graph *g, int v)
{
	int i;
	g->visited[v - 1] = 1;
	printf("vertex %d\n", v);
	for (i = 0; i < g->nvertices; i++)
	{
		if (g->matrix[(v - 1)*g->nvertices + i] > 0 && g->visited[i] == 0)
		{
			graph_dfs(g, i + 1);
		}
	}
}
void graph_bfs(struct graph *g, int v)
{
	int i, j;
	struct queue1 *q;
	for (i = 0; i < g->nvertices; i++)
	{
		g->visited[i] = 0;
	}
	q = queue_create();
	g->visited[v - 1] = 1;
	printf("Vertex%d\n", v);
	queue_enqueue(q, v - 1);
	while (queue_size(q) > 0)
	{
		i = queue_dequeue(q);
		for (j = 0; j < g->nvertices; j++)
		{
			if (g->matrix[i*g->nvertices + j] > 0 && g->visited[j] == 0)
			{
				queue_enqueue( q,j);
				g->visited[j] = 1;
				printf("Vertex%d\n", j + 1);
			}
		}
	}
	queue_free(q);
}

struct AdjListNode
{
	int dest;
	struct AdjListNode* next;
};

// A structure to represent an adjacency list
struct AdjList
{
	struct AdjListNode *head;  // pointer to head node of list
};

// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct GraphList
{
	int V;
	struct AdjList* array;
};

// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest)
{
	struct AdjListNode* newNode =
		(struct AdjListNode*) malloc(sizeof(struct AdjListNode));
	newNode->dest = dest;
	newNode->next = NULL;
	return newNode;
}

// A utility function that creates a graph of V vertices
struct GraphList* createGraph(int V)
{
	struct GraphList* graph = (struct GraphList*) malloc(sizeof(struct GraphList));
	graph->V = V;

	// Create an array of adjacency lists.  Size of array will be V
	graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));

	// Initialize each adjacency list as empty by making head as NULL
	int i;
	for (i = 0; i < V; ++i)
		graph->array[i].head = NULL;

	return graph;
}

// Adds an edge to an undirected graph
void addEdge(struct GraphList* graph, int src, int dest)
{
	// Add an edge from src to dest.  A new node is added to the adjacency
	// list of src.  The node is added at the begining
	struct AdjListNode* newNode = newAdjListNode(dest);
	newNode->next = graph->array[src].head;
	graph->array[src].head = newNode;

	// Since graph is undirected, add an edge from dest to src also
	newNode = newAdjListNode(src);
	newNode->next = graph->array[dest].head;
	graph->array[dest].head = newNode;
}
// A utility function to print the adjacenncy list representation of graph
void printGraph(struct GraphList* graph)
{
	int v;
	for (v = 0; v < graph->V; ++v)
	{
		struct AdjListNode* pCrawl = graph->array[v].head;
		printf("\n Adjacency list of vertex %d\n head ", v);
		while (pCrawl)
		{
			printf("-> %d", pCrawl->dest);
			pCrawl = pCrawl->next;
		}
		printf("\n");
	}
}
int dijk(int A, int B, struct graph *g) 
{
	
	int dist[11];
	//vector<bool> vis(g->nvertices );

	for (int i = 0; i <= g->nvertices; ++i) {
		dist[i] = 1;
		
	}
	dist[A] = 0;
	g->visited[A] = 1;
	for (int i = 1; i <= g->nvertices; ++i) {
		int cur = 0;
		for (int j = 1; j <= g->nvertices; ++j) {
			if (g->visited[j]==1) continue;
			if (cur == 0 || dist[j] < dist[cur]) {
				cur = j;
			}
		}

		g->visited[cur] = 1;
		for (int j = 1; j <= g->nvertices; ++j) {
			int path = dist[cur] + g->matrix[cur*g->nvertices + j];
			//*if (path <= dist[j]) {*//
				dist[j] = path+1;
			//}
		}
	}

	return dist[B];
}
int main()
{
	struct  graph *g;
	g = graph_create(10);
	clock_t start_time = clock(); 	
	graph_set_edge(g, 1, 2, 1);
	graph_set_edge(g, 1, 4, 1);
	graph_set_edge(g, 1, 5, 1);
	graph_set_edge(g, 2, 5, 1);
	graph_set_edge(g, 2, 3, 1);
	graph_set_edge(g, 3, 5, 1);
	graph_set_edge(g, 3, 4, 1);
	graph_set_edge(g, 3, 8, 1);
	graph_set_edge(g, 4, 5, 1);
	graph_set_edge(g, 4, 6, 1);
	graph_set_edge(g, 6, 7, 1);
	graph_set_edge(g, 7, 8, 1);
	graph_set_edge(g, 8, 9, 1);
	graph_set_edge(g, 8, 10, 1);
	clock_t end_time = clock()-start_time;
	
	 // искомое время
	cout << "time set_edge from adjacency matrix" << double(end_time)<<endl;
	printf("DFS:\n");
	graph_dfs(g, 2);
	printf("BFS:\n");
	graph_bfs(g, 2);
	int ans = dijk(2, 10, g);
	printf(" value of path from 2 to 10 %d\n", ans);
	graph_free(g);

	// adj list
	struct GraphList* graph = createGraph(10);
	clock_t start_time2 = clock();
	addEdge(graph, 1, 2);
	addEdge(graph, 1, 4);
	addEdge(graph, 1, 5);
	addEdge(graph, 2, 5);
	addEdge(graph, 2, 3);
	addEdge(graph, 3, 5);
	addEdge(graph, 3, 4);
	addEdge(graph, 3, 8);
	addEdge(graph, 4, 5);
	addEdge(graph, 4, 6);
	addEdge(graph, 6, 7);
	addEdge(graph, 7, 8);
	addEdge(graph, 8, 9);
	addEdge(graph, 8, 10);
	clock_t end_time2 = clock()-start_time2;

	//cout << "time set_edge from adjacency list" << double(end_time2) << endl;
	// print the adjacency list representation of the above graph
	printGraph(graph);

	system("pause");
    return 0;
}


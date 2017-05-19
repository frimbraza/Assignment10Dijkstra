

#ifndef GRAPH
#define GRAPH

#include <string>
#include <vector>
#include <iostream>

#include "heap.h"

using namespace std;

struct Node
{
	int dest;
	int weight;

	Node* next;
	Node(int inputDest, int inputWeight)
		: dest(inputDest), weight(inputWeight), next(NULL) {}
};

struct AdjList
{
	Node* head;
};

struct Edge
{
	int src;
	int dest;
	int DistWeight;
	void operator=(const Edge& other) 
	{
		src = other.src;
		dest = other.dest;
		DistWeight = other.DistWeight;
	}
	Edge() {}
	Edge(int inputSrc, int inputDest, int inputWeight)
		: src(inputSrc), dest(inputDest), DistWeight(inputWeight) {}
};

struct EdgeComparator
{
	bool operator()(Edge first, Edge second)
	{
		return first.DistWeight < second.DistWeight;
	}
};

class Graph
{
public:
	Graph(const vector<string>& vertices);
	void addEdge(string src, string dest, int dist);
	void printAdjList();
	void printDijkstras(string start);
	void printMST();

private:
	vector<string> verticeList;
	AdjList* adjListArray;

	vector<Edge> edgeList;

	void addAdjEdgesToFrontier(Heap<Edge,EdgeComparator>& frontier, bool visitedNodes[], int index, int maxDistArray[]);
	bool formCycle(vector<int> clusterList[], Edge edge);
	void updateCluster(vector<int> clusterList[], Edge edge);
};

#endif // !GRAPH

#include "graph.h"

Graph::Graph(const vector<string>& vertices)
{
	verticeList = vertices;
	adjListArray = new AdjList[verticeList.size()];
	for (int i = 0; i < verticeList.size(); ++i)
	{
		adjListArray[i].head = NULL;
	}
}

void Graph::addEdge(string src, string dest, int dist)
{
	int srcIndex, destIndex;
	for (int i = 0; i < verticeList.size(); ++i)
	{
		if (src == verticeList[i])
			srcIndex = i;
		if (dest == verticeList[i])
			destIndex = i;
	}

	Node* newNode = new Node(destIndex, dist);
	newNode->next = adjListArray[srcIndex].head;
	adjListArray[srcIndex].head = newNode;
	// Do the same because it is bi-directional
	newNode = new Node(srcIndex, dist);
	newNode->next = adjListArray[destIndex].head;
	adjListArray[destIndex].head = newNode;

	Edge theEdge(srcIndex, destIndex, dist);
	edgeList.push_back(theEdge);

}

void Graph::printAdjList()
{
	for (int i = 0; i < verticeList.size(); ++i)
	{
		cout << verticeList[i];
		Node* walker = adjListArray[i].head;
		while (walker)
		{
			string destination = verticeList[walker->dest];
			cout << " -> " << destination << ":" << walker->weight;
			walker = walker->next;
		}
		cout << endl;
	}
}

void Graph::printDijkstras(string start)
{
	int* maxDistArray = new int[verticeList.size()];					// Max Distance Array
	vector<string>* pathArray = new vector<string>[verticeList.size()];	// Paths for each Index
	Heap<Edge,EdgeComparator> frontier;												// Vector holding edge frontier
	bool* visitedNodes = new bool[verticeList.size()];					// Vector bool check visited nodes

	for (int i = 0; i < verticeList.size(); ++i)	// Set all visited nodes to false
		visitedNodes[i] = false;

	for (int i = 0; i < verticeList.size(); ++i)	// Set the max distance array all = -1
		maxDistArray[i] = -1;

	int startIndex;									// The index of the node to start with
	for (int i = 0; i < verticeList.size(); ++i)	// Search for index of the node to start with
	{
		if (verticeList[i] == start)
			startIndex = i;
	}

	maxDistArray[startIndex] = 0;								// Set max distance of the start index to 0
	pathArray[startIndex].push_back(verticeList[startIndex]);	// Set the path for the start index
	visitedNodes[startIndex] = true;							// Set the visited Node to true
			
	// Fill the frontier with adjacent edges of the start node
	addAdjEdgesToFrontier(frontier, visitedNodes, startIndex, maxDistArray);

	// Emptying the frontier && Performing Dijkstras
	while (!frontier.isEmpty())
	{
		Edge shortEdge = frontier.remove();
		if (!visitedNodes[shortEdge.dest])
		{
			// add to distance, path, and visited array/vectors
			maxDistArray[shortEdge.dest] = shortEdge.DistWeight;
			
			for (int i = 0; i < pathArray[shortEdge.src].size(); ++i)				// Add the past path to the src
			{
				pathArray[shortEdge.dest].push_back(pathArray[shortEdge.src][i]);
			}
			pathArray[shortEdge.dest].push_back(verticeList[shortEdge.dest]);

			visitedNodes[shortEdge.dest] = true;

			// add new edges to frontier
			addAdjEdgesToFrontier(frontier, visitedNodes, shortEdge.dest, maxDistArray);
		}

	}

	for (int i = 0; i < verticeList.size(); ++i)
	{
		cout << verticeList[i] << " " << maxDistArray[i] << " ";
		cout << "{ " << pathArray[i][0];
		for (int j = 1; j < pathArray[i].size(); ++j)
			cout << ", " << pathArray[i][j];
		cout << " }" << endl;
	}
	
}

void Graph::printMST()
{
	vector<Edge> MSTfinal;							// The Final MST
	Heap<Edge, EdgeComparator> minHeapEdgeList;		// The Heap to store all the edges, sorted with min on top
	for (int i = 0; i < edgeList.size(); ++i)		// Fill the Heap
	{
		minHeapEdgeList.insert(edgeList[i]);
	}
	
	vector<int>* clusterList = new vector<int>[verticeList.size()];
	for (int i = 0; i < verticeList.size(); ++i)
	{
		clusterList[i].push_back(i);
	}

	while (MSTfinal.size() != verticeList.size() - 1)
	{
		// if smallEdge doesn't form a cycle
		Edge smallEdge = minHeapEdgeList.remove();
		if (!formCycle(clusterList, smallEdge))
		{
			// Add small edge to the final MST
			MSTfinal.push_back(smallEdge);
			// add small edge to the cluster, make sure to include previous clusters
			updateCluster(clusterList, smallEdge);
		}
	}

	cout << "--------Minimum Spanning Tree---------\n";
	int totalMiles = 0;
	for (int i = 0; i < MSTfinal.size(); ++i)
	{
		cout << i + 1 << ":("
			<< verticeList[MSTfinal[i].src] << ", " 
			<< verticeList[MSTfinal[i].dest] << ") " 
			<< MSTfinal[i].DistWeight 
			<< endl;
		totalMiles += MSTfinal[i].DistWeight;
	}
	cout << endl << "Total Mileage = " << totalMiles << endl;
	
}

void Graph::addAdjEdgesToFrontier(Heap<Edge,EdgeComparator>& frontier,
	bool visitedNodes[], int index, int maxDistArray[])
{
	Node* walker = adjListArray[index].head;	// Set walker to adjacency vector for start index
	while (walker)
	{
		if (!visitedNodes[walker->dest])
		{
			Edge newEdge(index, walker->dest, walker->weight + maxDistArray[index]);		// Make edge
			frontier.insert(newEdge);			// Add edge to frontier
		}
		walker = walker->next;					// Set walker to next adjacent edge
	}

}

bool Graph::formCycle(vector<int> clusterList[], Edge edge)
{
	for (int i = 0; i < clusterList[edge.src].size(); ++i)
	{
		if (clusterList[edge.src][i] == edge.dest)
			return true;
	}
	for (int i = 0; i < clusterList[edge.dest].size(); ++i)
	{
		if (clusterList[edge.dest][i] == edge.src)
			return true;
	}
	return false;
}

void Graph::updateCluster(vector<int> clusterList[], Edge edge)
{
	vector<int> srcCluster = clusterList[edge.src];
	vector<int> destCluster = clusterList[edge.dest];

	for (int i = 0; i < srcCluster.size(); ++i)
	{
		for (int j = 0; j < destCluster.size(); ++j)
			clusterList[srcCluster[i]].push_back(destCluster[j]);
	}

	for (int i = 0; i < destCluster.size(); ++i)
	{
		for (int j = 0; j < srcCluster.size(); ++j)
			clusterList[destCluster[i]].push_back(srcCluster[j]);
	}
}


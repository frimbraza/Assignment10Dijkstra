

#include <iostream>
#include <fstream>
#include <sstream>

#include "graph.h"

using namespace std;

void initCityList(vector<string> & cityList, const vector<vector<string>>& edgeList);
bool existInList(const vector<string>& list, string value);

int main()
{
	vector<vector<string>> edgeHolder;
	vector<string> cityList;

	string line;
	ifstream infile("Assignment10Map.txt");
	while (getline(infile, line))
	{
		istringstream iss(line);
		vector<string> stringEdge;
		string token;
		while (getline(iss, token, ','))
		{
			stringEdge.push_back(token);
		}
		edgeHolder.push_back(stringEdge);
	}
	infile.close();

	initCityList(cityList, edgeHolder);

	Graph travelMap(cityList);
	for (int i = 0; i < edgeHolder.size(); ++i)
	{
		int distance = stoi(edgeHolder[i][2]);
		travelMap.addEdge(edgeHolder[i][0], edgeHolder[i][1], distance);
	}

	travelMap.printDijkstras("Seattle");
	cout << endl << endl;
	travelMap.printMST();


	return 0;
}

void initCityList(vector<string> & cityList, const vector<vector<string>>& edgeList)
{
	for (int i = 0; i < edgeList.size(); ++i)
	{
		if (!existInList(cityList, edgeList[i][0]))
			cityList.push_back(edgeList[i][0]);
		if (!existInList(cityList, edgeList[i][1]))
			cityList.push_back(edgeList[i][1]);
	}
}

bool existInList(const vector<string>& list, string value)
{
	for (int i = 0; i < list.size(); ++i)
	{
		if (list[i] == value)
			return true;
	}
	return false;
}

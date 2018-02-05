#pragma once
#include "../Waypoint.h"
#include "../WaypointManager.h"
#include "../Commons.h"

#include <algorithm>

using namespace::std;

struct node
{
	float f;
	float g;
	float h;
	node* parent;
	Waypoint* waypoint;
};

class P016671ePathfinding
{
public:
	P016671ePathfinding();
	vector<Vector2D> Astar(Waypoint* start, Waypoint* end, Vector2D DestPos);
	int heuristic(Vector2D a, Vector2D b);
	~P016671ePathfinding();
private:
	vector<node*> availableNodes;
	vector<node*> openList;
	vector<node*> closedList;

	vector<Vector2D> path;

	node* childNode;
	node* currentNode;
	node* startNode;

	float f = 0.0f;
	float g = 0.0f;
	float h = 0.0f;
};


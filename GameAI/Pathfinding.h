#pragma once
#include "Waypoint.h"
#include "WaypointManager.h"
#include "Commons.h"

#include <algorithm>

using namespace::std;

struct node
{
	int f;
	int g;
	int h;
	node* parent;
	Waypoint* waypoint;
};

class Pathfinding
{
public:
	Pathfinding();
	vector<Vector2D> Astar(Waypoint* start, Waypoint* end, Vector2D DestPos);
	int heuristic(Vector2D a, Vector2D b);
	~Pathfinding();
};


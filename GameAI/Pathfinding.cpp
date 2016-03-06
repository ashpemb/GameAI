#include "Pathfinding.h"


Pathfinding::Pathfinding()
{
}

vector<Vector2D> Pathfinding::Astar(Waypoint* start, Waypoint* end, Vector2D DestPos)
{
	vector<node*> availableNodes;
	vector<node*> openList;
	vector<node*> closedList;

	vector<Vector2D> path;

	node* childNode;
	node* currentNode;
	node* startNode;

	int f = 0;
	int g = 0;
	int h = 0;

	for (int i = 0; i < WaypointManager::Instance()->GetAllWaypoints().size(); i++)
	{
		node* Node = new node;

		Node->f = 0;
		Node->g = 0;
		Node->h = 0;
		Node->parent = nullptr;
		Node->waypoint = WaypointManager::Instance()->GetWaypointWithID(i);

		availableNodes.push_back(Node);
	}

	for (int i = 0; i < availableNodes.size(); i++)
	{
		if (availableNodes[i]->waypoint == start)
		{
			startNode = availableNodes[i];
		}
	}

	openList.push_back(startNode);

	currentNode = startNode;

	cout << "start node is " << start->GetID() << "endnode is " << end->GetID() << endl;

	while (!openList.empty())
	{
		int lowestIndex = 0;
		for (int i = 0; i < openList.size(); i++)
		{
			if (openList[i]->f < openList[lowestIndex]->f)
			{
				lowestIndex = i;
			}
		}
		currentNode = openList[lowestIndex];

		if (currentNode->waypoint == end)
		{
			node* cur = currentNode;
			path.push_back(DestPos);
			while (cur->parent)
			{
				cout << cur->waypoint->GetID();
				path.push_back(cur->waypoint->GetPosition());
				cur = cur->parent;
			}

			cout << cur->waypoint->GetID();
			path.push_back(cur->waypoint->GetPosition());

			std::reverse(path.begin(), path.end());
			return path;
		}

		openList.erase(std::remove(openList.begin(), openList.end(), currentNode), openList.end());
		closedList.push_back(currentNode);

		vector<int> children = currentNode->waypoint->GetConnectedWaypointIDs();

		for each (int child in children)
		{
			for (int i = 0; i < availableNodes.size(); i++)
			{
				if (availableNodes[i]->waypoint->GetID() == child)
				{
					childNode = availableNodes[i];
				}
			}

			if (find(closedList.begin(), closedList.end(), childNode) != closedList.end()) //is it in closed list?????? who knows
			{
				continue;
			}

			g = currentNode->g + 1; //direct child

			bool bestG = false;

			if (find(openList.begin(), openList.end(), childNode) == openList.end())
			{
				bestG = true;
				childNode->h = heuristic(childNode->waypoint->GetPosition(), end->GetPosition());
				openList.push_back(childNode);
			}
			else if (g < childNode->g)
			{
				bestG = true;
			}

			if (bestG == true)
			{
				childNode->parent = currentNode;
				childNode->g = g;
				childNode->f = childNode->g + childNode->h;
			}
		}
	}
}

int Pathfinding::heuristic(Vector2D a, Vector2D b)
{
	int d1 = abs(b.x - a.x);
	int d2 = abs(b.y - a.y);

	return d1 + d2;
}

Pathfinding::~Pathfinding()
{
}

#include "PathfindingP016671e.h"


Pathfinding::Pathfinding()
{
}

vector<Vector2D> Pathfinding::Astar(Waypoint* start, Waypoint* end, Vector2D DestPos)
{
	closedList.clear();
	openList.clear();
	path.clear();

	//create all nodes in the scene
	for (int i = 0; i < WaypointManager::Instance()->GetAllWaypoints().size(); i++)
	{
		node* Node = new node;

		Node->f = 0.0f;
		Node->g = 0.0f;
		Node->h = 0.0f;
		Node->parent = nullptr;
		Node->waypoint = WaypointManager::Instance()->GetWaypointWithID(i);

		availableNodes.push_back(Node);
	}

	//creates start node with the passed in waypoint
	startNode = new node;
	startNode->waypoint = start;
	startNode->f = 0.0f;
	startNode->g = 0.0f;
	startNode->h = 0.0f;
	startNode->parent = nullptr;

	openList.push_back(startNode);

	currentNode = startNode;
	//outputs start and end node for debugging
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

		//if the current node is the end waypoint, return the path
		if (currentNode->waypoint == end)
		{
			node* cur = currentNode;
			path.push_back(DestPos);

			while (cur->parent)
			{
				cout << cur->waypoint->GetID() << " ";
				path.push_back(cur->waypoint->GetPosition());
				cur = cur->parent;
			}
			//output is opposite of path because its reversed after
			cout << cur->waypoint->GetID() << " ";
			path.push_back(cur->waypoint->GetPosition());

			//return path to be used by steering behaiviours
			std::reverse(path.begin(), path.end());
			return path;
		}

		//erase the open list push the current node onto the closed list
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

			g = currentNode->g + 1; //its a direct child so its +1 manhattan

			bool bestG = false;

			//calculate the hueristic vector for the child node if it is found in the open list
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

			//calculate the f score for the child
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
	int xVect = abs(b.x - a.x);
	int yVect = abs(b.y - a.y);

	return xVect + yVect;
}

Pathfinding::~Pathfinding()
{
}

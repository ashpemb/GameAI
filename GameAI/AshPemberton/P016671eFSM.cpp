#include "P016671eFSM.h"
#include "../Commons.h"
#include "../C2DMatrix.h"

StateMachine::StateMachine()
{
	steering = new SteeringP016671e();
	pathfinding = new Pathfinding();
	destPos = Vector2D(100.0f, 100.0f);
	path.push_back(WaypointManager::Instance()->GetWaypointWithID(0)->GetPosition());
	currentPath = 0;
	isPathfinding = false;
	arrivedNode = false;
	toTar = { 0, 0 };
	state = PATROL;
}

void StateMachine::ChangeState()
{

}

void StateMachine::UpdateState(BaseTank * tank, float deltaTime)
{
	switch (state)
	{
	case PATROL:
		tankWaypoint = findWaypoint(tank->GetCentralPosition());

		targetWaypoint = findWaypoint(destPos);

		path.clear();
		currentPath = 0;

		path = pathfinding->Astar(tankWaypoint, targetWaypoint, destPos);

		isPathfinding = true;
		steering->CalculateForce(deltaTime, path[currentPath], tank);

		toTar = path[currentPath] - tank->GetCentralPosition();

		if (isPathfinding == true)
		{
			if (toTar.Length() >= 10.0f)
			{
				arrivedNode = false;
			}
			if (arrivedNode == true && currentPath != path.size() - 1)
			{
				currentPath++;
			}
			if (toTar.Length() <= 10.0f && arrivedNode == false)
			{
				arrivedNode = true;
			}
			if ((destPos - tank->GetCentralPosition()).Length() <= 5.0f)
			{
				isPathfinding = false;
				steering->AStarOff();
				steering->Pathon();
				steering->SeekOn();
				steering->SeekToMouse(deltaTime, destPos, tank);

			}
		}
			steering->CalculateForce(deltaTime, path[currentPath], tank);
		
			break;

	case RUN:
		break;
	case CHASE:
		break;
	case COLLECT:
		break;
	case FIRE:
		break;
		
	}
}

Waypoint* findWaypoint(Vector2D pos)
{
	vector<Waypoint*> allWaypoints = WaypointManager::Instance()->GetAllWaypoints();
	Waypoint* closest = allWaypoints[0];
	for (int i = 0; i < allWaypoints.size(); i++)
	{
		if (pos.Distance(allWaypoints[i]->GetPosition()) < pos.Distance(closest->GetPosition()))
		{
			closest = allWaypoints[i];
		}
	}
	return closest;
}
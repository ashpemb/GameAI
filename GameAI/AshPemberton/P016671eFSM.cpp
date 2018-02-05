#include "P016671eFSM.h"
#include "../Commons.h"
#include "../C2DMatrix.h"

P016671eStateMachine::P016671eStateMachine()
{
	path.push_back(WaypointManager::Instance()->GetWaypointWithID(0)->GetPosition());
	currentPath = 0;
	isPathfinding = false;
	arrivedNode = false;
	state = PATROL;
	patroling = false;
	pickupFound = false;
	srand(time(NULL));
}

void P016671eStateMachine::ChangeState(TANK_FSM newState)
{
	steering->AllOff();
	state = newState;
	patroling = false;
}

void P016671eStateMachine::UpdateState(BaseTank * tank, float deltaTime)
{
	pickups = PickUpManager::Instance()->GetAllPickUps();
	vector<BaseTank*> enemiesISee = TankManager::Instance()->GetVisibleTanks(tank);
	waypoints = WaypointManager::Instance()->GetAllWaypoints();
	if (!pickups.empty() && pickupFound == false && chasing == false)
	{
		//cout << "state change COLLECT" << endl;
		ChangeState(COLLECT);
		pickupFound = true;
		chasing = false;
		mines = false;
	}
	if (pickups.empty() && pickupFound == true)
	{
		//cout << "state change Patrol" << endl;
		ChangeState(PATROL);
		tankWaypoint = findWaypoint(tank->GetCentralPosition());
		chasing = false;
		pickupFound = false;
		mines = false;
	}
	if (!enemiesISee.empty() && chasing == false)
	{
		//cout << "state change chase" ;
		ChangeState(CHASE);
		chasing = true;
		pickupFound = false;
		mines = false;
	}

	Vector2D centPos = tank->GetCentralPosition();
	switch (state)
	{
	case PATROL:
		stop = false;
		steering->AStarOn();

		if (!patroling)
		{
			patroling = true;
			path.clear();
			path.push_back(WaypointManager::Instance()->GetWaypointWithID(0)->GetPosition());
			path.push_back(WaypointManager::Instance()->GetWaypointWithID(1)->GetPosition());
			path.push_back(WaypointManager::Instance()->GetWaypointWithID(3)->GetPosition());
			path.push_back(WaypointManager::Instance()->GetWaypointWithID(5)->GetPosition());
			path.push_back(WaypointManager::Instance()->GetWaypointWithID(7)->GetPosition());
			path.push_back(WaypointManager::Instance()->GetWaypointWithID(6)->GetPosition());
			path.push_back(WaypointManager::Instance()->GetWaypointWithID(4)->GetPosition());
			path.push_back(WaypointManager::Instance()->GetWaypointWithID(2)->GetPosition());

			for (unsigned i = 0; i < path.size(); i++)
			{
				if (path[i] == findWaypoint(tank->GetCentralPosition())->GetPosition())
				{
					currentPath = i;
				}
			}

			//int random = rand() % (waypoints.size() - 1);
			//int targ = waypoints.at(random)->GetID();

			//path = pathfinding->Astar(findWaypoint(tank->GetCentralPosition()), WaypointManager::Instance()->GetWaypointWithID(targ), WaypointManager::Instance()->GetWaypointWithID(targ)->GetPosition());

			isPathfinding = true;
			steering->CalculateForce(deltaTime, path[currentPath], tank);
		}
		toTar = path[currentPath] - tank->GetCentralPosition();

		if (isPathfinding == true)
		{
			if (toTar.Length() >= 10.0f)
			{
				arrivedNode = false;
				mines = false;
			}
			if (arrivedNode == true && currentPath != path.size() - 1)
			{
				mines = true;
				currentPath++;
			}
			if (toTar.Length() <= 10.0f && arrivedNode == false)
			{
				mines = true;
				arrivedNode = true;
			}
			if ((path[path.size() - 1] - tank->GetCentralPosition()).Length() <= 10.0f)
			{
				mines = true;
				//patroling = false;
				currentPath = 0;
			}
		}
		steering->CalculateForce(deltaTime, path[currentPath], tank);

		break;

	case RUN:
		break;
	case CHASE:
		steering->AllOff();
		steering->SeekOn();
		steering->ObsAvoidOn();
		if (!enemiesISee.empty())
		{
			steering->CalculateForce(deltaTime, enemiesISee[0]->GetCentralPosition(), tank);

			//length = (tank->GetCentralPosition() - enemiesISee[0]->GetCentralPosition()).Length();
			//if (length <= 50)
			//{
			//	stop = true;
			//}
			//else
			//{
			//	stop = false;
			//}
		}
		
		if (enemiesISee.empty())
		{
			ChangeState(PATROL);
			chasing = false;
			break;
		}
		
		
		break;
	case COLLECT:
		stop = false;
		steering->AStarOn();
		if (!patroling)
		{
			currentPath = 0;
			
			if (!pickups.empty())
			{
				destPos = pickups[0]->GetCentralPosition();
			}

			tankWaypoint = findWaypoint(tank->GetCentralPosition());

			targetWaypoint = findWaypoint(destPos);
			path.clear();
			path = pathfinding->Astar(tankWaypoint, targetWaypoint, destPos);

			patroling = true;
			isPathfinding = true;
			steering->CalculateForce(deltaTime, path[currentPath], tank);
		}
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
				steering->SeekOn();
				steering->SeekToMouse(deltaTime, destPos, tank);
				patroling = false;
			}
		}
		steering->CalculateForce(deltaTime, path[currentPath], tank);

		break;
	case FIRE:
		break;
		
	}
}

Waypoint* P016671eStateMachine::findWaypoint(Vector2D pos)
{
	vector<Waypoint*> allWaypoints = WaypointManager::Instance()->GetAllWaypoints();
	Waypoint* closest = allWaypoints[0];
	for (unsigned i = 0; i < allWaypoints.size(); i++)
	{
		if (pos.Distance(allWaypoints[i]->GetPosition()) < pos.Distance(closest->GetPosition()))
		{
			closest = allWaypoints[i];
		}
	}
	return closest;
}
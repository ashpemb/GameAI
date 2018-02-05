#include "../Commons.h"
#include "../TankManager.h"
#include <SDL.h>
#include <vector>
#include "../BaseTank.h"
#include "PathfindingP016671e.h"
#include "SteeringP016671e.h"
#include "../Waypoint.h"
#include "../WaypointManager.h"
#include "../PickUpManager.h"
#include <time.h>

using namespace::std;

enum TANK_FSM
{
	PATROL,
	RUN,
	FIRE,
	CHASE,
	COLLECT,
};

class P016671eStateMachine
{

public:

	P016671eStateMachine();

	TANK_FSM IsInState(){ return state; };
	bool DropMine(){ return mines; };
	bool Stop(){ return stop; };
	void ChangeState(TANK_FSM newState);

	void UpdateState(BaseTank * tank, float deltaTime);

	Waypoint* findWaypoint(Vector2D pos);

	SteeringP016671e* steering = new SteeringP016671e();
	P016671ePathfinding* pathfinding = new P016671ePathfinding();

private:

	TANK_FSM state;
	Vector2D destPos;
	vector<Vector2D> path;

	int currentPath;
	bool isPathfinding;
	bool arrivedNode;
	Waypoint* tankWaypoint = nullptr;
	vector<Waypoint*> waypoints;
	vector<GameObject*> pickups;
	Waypoint* targetWaypoint = nullptr;
	Vector2D toTar;
	bool patroling;
	bool pickupFound;
	bool chasing = false;
	double length;
	bool mines = false;
	bool stop = false;

};
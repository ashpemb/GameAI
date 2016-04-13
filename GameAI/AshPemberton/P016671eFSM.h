#include "../Commons.h"
#include "../TankManager.h"
#include <SDL.h>
#include <vector>
#include "../BaseTank.h"
#include "PathfindingP016671e.h"
#include "SteeringP016671e.h"
#include "../Waypoint.h"
#include "../WaypointManager.h"

using namespace::std;

enum TANK_FSM
{
	PATROL,
	RUN,
	FIRE,
	CHASE,
	COLLECT,
};

class StateMachine
{

public:
	SteeringP016671e* steering;
	StateMachine();

	TANK_FSM IsInState(){return state;};

	void ChangeState();

	void UpdateState(BaseTank * tank, float deltaTime);

	Waypoint* findWaypoint(Vector2D pos);

private:

	TANK_FSM state;
	Vector2D destPos;
	vector<Vector2D> path;
	Pathfinding* pathfinding;
	int currentPath;
	bool isPathfinding;
	bool arrivedNode;
	Waypoint* tankWaypoint = nullptr;
	Waypoint* targetWaypoint = nullptr;
	Vector2D toTar;

};
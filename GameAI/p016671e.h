#ifndef P016671ETANK_H
#define P016671ETANK_H

#include "BaseTank.h"
#include "SteeringP016671e.h"
#include <SDL.h>
#include "Commons.h"
#include "Waypoint.h"
#include "WaypointManager.h"
#include "Pathfinding.h"



//---------------------------------------------------------------

class P016671eTank : protected BaseTank
{
	//---------------------------------------------------------------
public:
	P016671eTank(SDL_Renderer* renderer, TankSetupDetails details);
	~P016671eTank();

	void ChangeState(BASE_TANK_STATE newState);

	void Update(float deltaTime, SDL_Event e);

	Waypoint*  findWaypoint(Vector2D);


	//---------------------------------------------------------------
protected:
	void	MoveInHeadingDirection(float deltaTime);

private:
	TURN_DIRECTION  mTankTurnDirection;
	bool			mTankTurnKeyDown;
	MOVE_DIRECTION  mTankMoveDirection;
	bool			mTankMoveKeyDown;
	TURN_DIRECTION  mManTurnDirection;
	bool			mManKeyDown;
	bool			mFireKeyDown;
	SteeringP016671e* steering;
	POINT p;
	Vector2D cursorPos;
	float mMouseX;
	float mMouseY;
	vector<Vector2D> path;
	Pathfinding* pathfinding;
	int currentPath;
	bool isPathfinding;
	bool arrivedNode;
};

//---------------------------------------------------------------

#endif //CONTROLLEDTANK_H
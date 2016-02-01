#ifndef P016671ETANK_H
#define P016671ETANK_H

#include "BaseTank.h"
#include "SteeringP016671e.h"
#include <SDL.h>
#include "Commons.h"

//---------------------------------------------------------------

class P016671eTank : protected BaseTank
{
	//---------------------------------------------------------------
public:
	P016671eTank(SDL_Renderer* renderer, TankSetupDetails details);
	~P016671eTank();

	void ChangeState(BASE_TANK_STATE newState);

	void Update(float deltaTime, SDL_Event e);

	void RotateHeadingByRadian(double radian, int sign);

	void ProcessInput(float deltaTime);

	void SeekToMouse(float deltaTime);

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
	bool			mMouseLeftDown;
	SteeringP016671e* steering;
	POINT p;
	Vector2D cursorPos;
};

//---------------------------------------------------------------

#endif //CONTROLLEDTANK_H
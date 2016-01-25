#ifndef P016671ETANK_H
#define P016671ETANK_H

#include "BaseTank.h"
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
};

//---------------------------------------------------------------

#endif //CONTROLLEDTANK_H
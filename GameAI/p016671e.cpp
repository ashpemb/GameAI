#include "p016671e.h"
#include "TankManager.h"
#include "Commons.h"
#include "C2DMatrix.h"

//--------------------------------------------------------------------------------------------------

P016671eTank::P016671eTank(SDL_Renderer* renderer, TankSetupDetails details)
	: BaseTank(renderer, details)
{
	mTankTurnDirection  = DIRECTION_UNKNOWN;
	mTankTurnKeyDown	= false;
	mTankMoveDirection	= DIRECTION_NONE;
	mTankMoveKeyDown	= false;
	mManTurnDirection   = DIRECTION_UNKNOWN;
	mManKeyDown			= false;
	mFireKeyDown		= false;
	steering = new SteeringP016671e();
	//steering->SeekOn();
	//steering->FleeOn();
	//steering->ArriveOn();
	steering->PursuitOn();
}

//--------------------------------------------------------------------------------------------------

P016671eTank::~P016671eTank()
{
	delete steering;
}

//--------------------------------------------------------------------------------------------------

void P016671eTank::ChangeState(BASE_TANK_STATE newState)
{
	BaseTank::ChangeState(newState);
}

//--------------------------------------------------------------------------------------------------

void P016671eTank::Update(float deltaTime, SDL_Event e)
{
	if (steering->GetPursuit() != true)
	{
		switch (e.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			mMouseX = e.button.x;
			mMouseY = e.button.y;

			cursorPos = { mMouseX, mMouseY };

			steering->CalculateForce(deltaTime, cursorPos, GetCentrePosition(), GetVelocity(), GetMaxSpeed());
			break;
		}
		if (cursorPos.x != 0.0f && cursorPos.y != 0.0f)
		{
			steering->CalculateForce(deltaTime, cursorPos, GetCentrePosition(), GetVelocity(), GetMaxSpeed());
		}
		Vector2D click = cursorPos - GetCentrePosition();
		if (steering->GetArrive() == true && click.Length() < 1)
		{
			mVelocity.x = 0;
			mVelocity.y = 0;
		}
	}
	else
	{
		switch (e.type)
		{
		case SDL_MOUSEMOTION:
			mMouseX = e.motion.x;
			mMouseY = e.motion.y;

			cursorPos = { mMouseX, mMouseY };

			steering->CalculateForce(deltaTime, cursorPos, GetCentrePosition(), GetVelocity(), GetMaxSpeed());
			break;
		}
		if (cursorPos.x != 0.0f && cursorPos.y != 0.0f)
		{
			steering->CalculateForce(deltaTime, cursorPos, GetCentrePosition(), GetVelocity(), GetMaxSpeed());
		}
	}

	//Call parent update.
	BaseTank::Update(deltaTime, e);
}

//--------------------------------------------------------------------------------------------------

void P016671eTank::MoveInHeadingDirection(float deltaTime)
{
	//Get the force that propels in current heading.
	Vector2D force = steering->GetForce();

	//Acceleration = Force/Mass
	Vector2D acceleration = force/GetMass();

	//Update velocity.
	mVelocity += acceleration * deltaTime;
	
	//Don't allow the tank does not go faster than max speed.
	mVelocity.Truncate(GetMaxSpeed()); //TODO: Add Penalty for going faster than MAX Speed.

	//Finally, update the position.
	Vector2D newPosition = GetPosition();
		newPosition.x += mVelocity.x*deltaTime;
		newPosition.y += (mVelocity.y/**-1.0f*/)*deltaTime;	//Y flipped as adding to Y moves down screen.
	SetPosition(newPosition);
	
	Vector2D ahead = mVelocity;
	if (ahead.x == 0.0f && ahead.y == 0.0f)
		ahead = mHeading;
	Vec2DNormalize(ahead);
	
	if (steering->GetAllowRotate() == true)
	RotateHeadingToFacePosition(GetCentrePosition() + ahead * 10.0f);
}

//--------------------------------------------------------------------------------------------------

#include "P016671eTank.h"
#include "../TankManager.h"
#include "../Commons.h"
#include "../C2DMatrix.h"

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
	stateMachine = new P016671eStateMachine;
}

//--------------------------------------------------------------------------------------------------

P016671eTank::~P016671eTank()
{
	
}

//--------------------------------------------------------------------------------------------------

void P016671eTank::ChangeState(BASE_TANK_STATE newState)
{
	BaseTank::ChangeState(newState);
}

//--------------------------------------------------------------------------------------------------

void P016671eTank::Update(float deltaTime, SDL_Event e)
{
	stateMachine->UpdateState(this, deltaTime);
	if (stateMachine->IsInState() == CHASE)
	{
		if (mTanksICanSee.size() != 0)
		{

			//Rotate man to face enemy tank.
			Vector2D toTarget = mTanksICanSee[0]->GetCentralPosition() - GetCentralPosition();
			toTarget.Normalize();
			//Determine the angle between the heading vector and the target.
			double angle = acos(mManFireDirection.Dot(toTarget));

			//Ensure angle does not become NaN and cause the tank to disappear.
			if (angle != angle)
				angle = 0.0f;
			double dot = toTarget.Dot(mManFireDirection);
			if (dot < 0.999f)
			{
				RotateManByRadian(angle, mManFireDirection.Sign(toTarget), deltaTime);
				
			}
			else
			{
				this->FireABullet();
			}



			this->FireRockets();
			if (stateMachine->Stop())
			mVelocity = { 0, 0 };
		}
	}
	if (stateMachine->DropMine() == true)
	{
		this->DropAMine();
	}
	//Call parent update.
	BaseTank::Update(deltaTime, e);
}

//--------------------------------------------------------------------------------------------------

void P016671eTank::MoveInHeadingDirection(float deltaTime)
{
	//Get the force that propels in current heading.
	Vector2D force = stateMachine->steering->GetForce();

	//Acceleration = Force/Mass
	Vector2D acceleration = force/GetMass();

	if (force == Vector2D(0.0f, 0.0f))
	{
		mVelocity = { 0.0f, 0.0f };
	}

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
	
	if (stateMachine->steering->GetAllowRotate() == true)
	RotateHeadingToFacePosition(GetCentralPosition() + ahead * 10.0f, deltaTime * 6.0f);
}

//--------------------------------------------------------------------------------------------------


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
	//steering->ObsAvoidOn();
	//steering->AStarOn();
	//steering->SeekOn();
	//steering->FleeOn();
	//steering->ArriveOn();
	//steering->PursuitOn();
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
	/*if (steering->GetPursuit() != true && steering->GetAStar() != true)
	{
		switch (e.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			mMouseX = e.button.x;
			mMouseY = e.button.y;

			cursorPos = { mMouseX, mMouseY };

			steering->CalculateForce(deltaTime, cursorPos, this);
			break;
		}
		if (cursorPos.x != 0.0f && cursorPos.y != 0.0f)
		{
			steering->CalculateForce(deltaTime, cursorPos, this);
		}
		Vector2D click = cursorPos - GetCentralPosition();
		if (steering->GetArrive() == true && click.Length() < 1)
		{
			mVelocity.x = 0;
			mVelocity.y = 0;
		}
	}
	if (steering->GetAStar())
	{
		switch (e.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			mMouseX = e.button.x;
			mMouseY = e.button.y;

			cursorPos = { mMouseX, mMouseY };

			steering->SeekOff();

			Waypoint* tankWaypoint = findWaypoint(this->GetCentralPosition());

			Waypoint* targetWaypoint = findWaypoint(cursorPos);

			path.clear();
			currentPath = 0;

			path = pathfinding->Astar(tankWaypoint, targetWaypoint, cursorPos);

			isPathfinding = true;
			steering->CalculateForce(deltaTime, path[currentPath], this);

			break;
		}
		
			Vector2D toTar = path[currentPath] - this->GetCentralPosition();

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
				if ((cursorPos - this->GetCentralPosition()).Length() <= 5.0f)
				{
					isPathfinding = false;
					steering->AStarOff();
					steering->Pathon();
					steering->SeekOn();
					steering->SeekToMouse(deltaTime, cursorPos, this);

				}
				steering->CalculateForce(deltaTime, path[currentPath], this);
			}
			
		
	}
	if (steering->GetPursuit())
	{
		switch (e.type)
		{
		case SDL_MOUSEMOTION:
			mMouseX = e.motion.x;
			mMouseY = e.motion.y;

			cursorPos = { mMouseX, mMouseY };

			steering->CalculateForce(deltaTime, cursorPos, this);
			break;
		}
		if (cursorPos.x != 0.0f && cursorPos.y != 0.0f)
		{
			steering->CalculateForce(deltaTime, cursorPos, this);
		}
	}
	if (steering->GetPathOff())
	{
		switch (e.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			mMouseX = e.button.x;
			mMouseY = e.button.y;

			cursorPos = { mMouseX, mMouseY };
			steering->SeekOff();
			steering->AStarOn();
			steering->Pathoff();

			Waypoint* tankWaypoint = findWaypoint(this->GetCentralPosition());

			Waypoint* targetWaypoint = findWaypoint(cursorPos);

			path.clear();
			currentPath = 0;

			path = pathfinding->Astar(tankWaypoint, targetWaypoint, cursorPos);

			isPathfinding = true;
			steering->CalculateForce(deltaTime, path[currentPath], this);

			break;
		}

		Vector2D toTar = path[currentPath] - this->GetCentralPosition();

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
			if ((cursorPos - this->GetCentralPosition()).Length() <= 5.0f)
			{
				isPathfinding = false;
				steering->AStarOff();
				steering->Pathon();
				steering->SeekOn();
				steering->SeekToMouse(deltaTime, cursorPos, this);
			}
			steering->CalculateForce(deltaTime, path[currentPath], this);
		}
		
	}*/
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
	RotateHeadingToFacePosition(GetCentralPosition() + ahead * 10.0f, deltaTime);
}

//--------------------------------------------------------------------------------------------------


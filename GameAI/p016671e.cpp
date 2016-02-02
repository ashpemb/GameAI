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
	ProcessInput(deltaTime);
	switch (e.type)
	{
	case SDL_MOUSEMOTION:
		mMouseX = e.motion.x;
		mMouseY = e.motion.y;
		
		//cout << mMouseX << ":" << mMouseY << endl;
		Vector2D cursorPos = { mMouseX, mMouseY };

		SeekToMouse(deltaTime);
		RotateHeadingToFacePosition(cursorPos);
		break;
	}
	
	//Call parent update.
	BaseTank::Update(deltaTime, e);
}

//--------------------------------------------------------------------------------------------------

void P016671eTank::MoveInHeadingDirection(float deltaTime)
{
	//Get the force that propels in current heading.
	Vector2D force = (mHeading*mCurrentSpeed)-mVelocity;

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
}

//--------------------------------------------------------------------------------------------------

void P016671eTank::RotateHeadingByRadian(double radian, int sign)
{
	//Clamp the amount to turn to the max turn rate.
	if (radian > mMaxTurnRate) 
		radian = mMaxTurnRate;
	else if(radian < -mMaxTurnRate)
		radian = -mMaxTurnRate;
	//IncrementTankRotationAngle(RadsToDegs(radian));
    mRotationAngle += RadsToDegs(radian)*sign;

	//Usee a rotation matrix to rotate the player's heading
	C2DMatrix RotationMatrix;
  
	//Calculate the direction of rotation.
	RotationMatrix.Rotate(radian * sign);	
	//Get the new heading.
	RotationMatrix.TransformVector2Ds(mHeading);

	//cout << "RotateHeadingByRadian -- Heading x = " << mHeading.x << " y = " << mHeading.y << endl;

	//Get the new velocity.
	RotationMatrix.TransformVector2Ds(mVelocity);

	//Side vector must always be perpendicular to the heading.
	mSide = mHeading.Perp();
}

//--------------------------------------------------------------------------------------------------

void P016671eTank::ProcessInput(float deltaTime)
{
	//if ((GetKeyState(VK_LBUTTON) != 0))
	//{
	//	GetCursorPos(&p);
	//	cursorPos.x = p.x;
	//	cursorPos.y = p.y;
	//	SeekToMouse(deltaTime);
	//}

}

void P016671eTank::SeekToMouse(float deltaTime)
{
	Vector2D Seeking;
	Seeking.x = cursorPos.x;
	Seeking.y = cursorPos.y;
	Vector2D SeekToVelocity = Vec2DNormalize(steering->Seek(cursorPos, GetCentrePosition(), GetVelocity(), GetMaxSpeed()));

	Vector2D Seek = (SeekToVelocity - Seeking);
	
	MoveInHeadingDirection(deltaTime);
}
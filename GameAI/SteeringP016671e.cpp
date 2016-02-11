#include "SteeringP016671e.h"

SteeringP016671e::SteeringP016671e()
{
	fleeRadius = 200;
	arriveRadius = 200;
	lastPos.x = 0;
	lastPos.y = 0;
	DoAllowRotate();
}
Vector2D SteeringP016671e::Seek(Vector2D targetPos, Vector2D tankPos, Vector2D velocity, double maxSpeed)
{
	Vector2D VectorToTarget = Vec2DNormalize(targetPos - tankPos) * maxSpeed;
	return (VectorToTarget - velocity);
}

Vector2D SteeringP016671e::SeekToMouse(float deltaTime, Vector2D cursorPos, Vector2D tankPos, Vector2D velocity, double maxSpeed)
{
	Vector2D force = Seek(cursorPos, tankPos, velocity, maxSpeed);
	return force;
}

Vector2D SteeringP016671e::FleeFromMouse(float deltaTime, Vector2D cursorPos, Vector2D tankPos, Vector2D velocity, double maxSpeed)
{

	Vector2D click = cursorPos - tankPos;
	if (click.Length() < fleeRadius)
	{
		Vector2D force = Seek(cursorPos, tankPos, velocity, maxSpeed);
		return force;
	}
	return Vector2D(0, 0);
}

Vector2D SteeringP016671e::ObstacleAvoid(float deltaTime, Vector2D cursorPos, Vector2D tankPos, Vector2D velocity, double maxSpeed)
{
	
}

Vector2D SteeringP016671e::ArriveAtMouse(float deltaTime, Vector2D cursorPos, Vector2D tankPos, Vector2D velocity, double maxSpeed)
{
	DoAllowRotate();
	Vector2D click = cursorPos - tankPos;
	double distance = click.Length();
	if (distance > 20)
	{
		double speed = distance / deceleration;
		speed = min(speed, maxSpeed);
		Vector2D force = Seek(cursorPos, tankPos, velocity, speed);
		
		if (distance < 5)
		{
			DontAllowRotate();
		}
		return force;
	}
	if (distance < 5)
	{
		DontAllowRotate();
	}
	return Vector2D(0, 0);
}

Vector2D SteeringP016671e::PursuitMouse(float deltaTime, Vector2D cursorPos, Vector2D tankPos, Vector2D velocity, double maxSpeed)
{
	if (lastPos.x == 0 && lastPos.y == 0)
	{
		lastPos = cursorPos;
	}
	Vector2D heading = Vec2DNormalize(cursorPos - lastPos);
	lastPos = cursorPos;
	Vector2D pursuitPoint = cursorPos;
	if (heading.x != 0 && heading.y != 0)
	{
		pursuitPoint = cursorPos * (0.5 * heading);
	}
	
	Vector2D force = Seek(pursuitPoint, tankPos, velocity, maxSpeed);
	return force;
}

Vector2D SteeringP016671e::CalculateForce(float deltaTime, Vector2D cursorPos, Vector2D tankPos, Vector2D velocity, double maxSpeed)
{
	combinedForce = Vector2D(0, 0);
	if (isSeekOn == true)
	{
		combinedForce += SeekToMouse(deltaTime, cursorPos, tankPos, velocity, maxSpeed);
	}
	if (isFleeOn == true)
	{
		combinedForce -= FleeFromMouse(deltaTime, cursorPos, tankPos, velocity, maxSpeed);
	}
	if (isArriveOn == true)
	{
		combinedForce += ArriveAtMouse(deltaTime, cursorPos, tankPos, velocity, maxSpeed);
	}
	if (isPursuitOn == true)
	{
		combinedForce += PursuitMouse(deltaTime, cursorPos, tankPos, velocity, maxSpeed);
	}
	return combinedForce;
}

SteeringP016671e::~SteeringP016671e()
{

}
#include "SteeringP016671e.h"

SteeringP016671e::SteeringP016671e()
{
	fleeRadius = 200;
	arriveRadius = 200;
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
	Vector2D force = Seek(cursorPos, tankPos, velocity, maxSpeed);
	Vector2D click = cursorPos - tankPos;
	if (click.Length() > fleeRadius)
	{
		force.x = 0.0f;
		force.y = 0.0f;
	}
	return force;
}

//Vector2D SteeringP016671e::ObstacleAvoid(float deltaTime, Vector2D cursorPos, Vector2D tankPos, Vector2D velocity, double maxSpeed)
//{
//	return null;
//}

Vector2D SteeringP016671e::ArriveAtMouse(float deltaTime, Vector2D cursorPos, Vector2D tankPos, Vector2D velocity, double maxSpeed)
{
	DoAllowRotate();
	Vector2D click = cursorPos - tankPos;
	double distance = click.Length();
	if (distance > 5)
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

Vector2D SteeringP016671e::CalculateForce(float deltaTime, Vector2D cursorPos, Vector2D tankPos, Vector2D velocity, double maxSpeed)
{
	combinedForce = Vector2D(0, 0);
	switch (tankState)
	{
	case SEEKTOMOUSE:
		combinedForce += SeekToMouse(deltaTime, cursorPos, tankPos, velocity, maxSpeed);
		break;
	case FLEEFROMMOUSE:
		combinedForce -= FleeFromMouse(deltaTime, cursorPos, tankPos, velocity, maxSpeed);
		break;
	case ARRIVE:
		combinedForce += ArriveAtMouse(deltaTime, cursorPos, tankPos, velocity, maxSpeed);
		break;
	}
	return combinedForce;
}

SteeringP016671e::~SteeringP016671e()
{

}
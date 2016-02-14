#include "SteeringP016671e.h"

SteeringP016671e::SteeringP016671e()
{
	fleeRadius = 200;
	arriveRadius = 200;
	lastPos.x = 0;
	lastPos.y = 0;
	DoAllowRotate();
}
Vector2D SteeringP016671e::Seek(Vector2D targetPos, BaseTank * Tank)
{
	Vector2D VectorToTarget = Vec2DNormalize(targetPos - Tank->GetCentrePosition()) * Tank->GetMaxSpeed();
	return (VectorToTarget - Tank->GetVelocity());
}

Vector2D SteeringP016671e::SeekToMouse(float deltaTime, Vector2D cursorPos, BaseTank * Tank)
{
	Vector2D force = Seek(cursorPos, Tank);
	return force;
}

Vector2D SteeringP016671e::FleeFromMouse(float deltaTime, Vector2D cursorPos, BaseTank * Tank)
{

	Vector2D click = cursorPos - Tank->GetCentrePosition();
	if (click.Length() < fleeRadius)
	{
		Vector2D force = Seek(cursorPos, Tank);
		return force;
	}
	return Vector2D(0, 0);
}

Vector2D SteeringP016671e::ObstacleAvoid(float deltaTime, Vector2D cursorPos, BaseTank * Tank, const std::vector<GameObject*>& ObstacleM)
{
	
	Vector2D AvoidForce(0, 0);
	std::vector<GameObject*>::const_iterator cur0b = ObstacleM.begin();

	Vector2D AheadCheckLeft = Tank->GetCentrePosition() + Vec2DNormalize(Tank->GetVelocity()) * AvoidCheckDistance;
	Vector2D AheadCheckRight = Tank->GetCentrePosition() + Vec2DNormalize(Tank->GetVelocity()) * AvoidCheckDistance;

	for (int i = 0; i < ObstacleM.size(); i++)
	{

		if (Collisions::Instance()->PointInBox(AheadCheckLeft, ObstacleM[i]->GetAdjustedBoundingBox())) // SEEMS VERY WEAK FOR SOME REASON
		{
			AvoidForce = AheadCheckLeft - ObstacleM[i]->GetCentralPosition();
			AvoidForce = Vec2DNormalize(AvoidForce) * Tank->GetMaxForce();
		}
	}
	return AvoidForce;

}

Vector2D SteeringP016671e::ArriveAtMouse(float deltaTime, Vector2D cursorPos, BaseTank * Tank)
{
	DoAllowRotate();
	Vector2D click = cursorPos - Tank->GetCentrePosition();
	double distance = click.Length();
	if (distance > 20)
	{
		double speed = distance / deceleration;
		speed = min(speed, Tank->GetMaxSpeed());
		Vector2D force = Seek(cursorPos, Tank);
		
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

Vector2D SteeringP016671e::PursuitMouse(float deltaTime, Vector2D cursorPos, BaseTank * Tank)
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
		pursuitPoint = cursorPos * (/*0.5 * */heading);
	}
	
	Vector2D force = Seek(pursuitPoint, Tank);
	return force;
}

Vector2D SteeringP016671e::CalculateForce(float deltaTime, Vector2D cursorPos, BaseTank * Tank)
{
	combinedForce = Vector2D(0, 0);
	if (isSeekOn)
	{
		combinedForce += SeekToMouse(deltaTime, cursorPos, Tank);
	}
	if (isFleeOn)
	{
		combinedForce -= FleeFromMouse(deltaTime, cursorPos, Tank);
	}
	if (isArriveOn)
	{
		combinedForce += ArriveAtMouse(deltaTime, cursorPos, Tank);
	}
	if (isPursuitOn)
	{
		combinedForce += PursuitMouse(deltaTime, cursorPos, Tank);
	}
	if (isObsAvoidOn)
	{
		std::vector<GameObject*>& ObstacleM = ObstacleManager::Instance()->GetObstacles();
		combinedForce += ObstacleAvoid(deltaTime, cursorPos, Tank, ObstacleM);
	}
	return combinedForce;
}

SteeringP016671e::~SteeringP016671e()
{

}
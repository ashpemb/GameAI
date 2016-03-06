#include "SteeringP016671e.h"

SteeringP016671e::SteeringP016671e()
{
	fleeRadius = 200;
	arriveRadius = 200;
	speed = 0;
	lastPos.x = 0;
	lastPos.y = 0;
	DoAllowRotate();
}
Vector2D SteeringP016671e::Seek(Vector2D targetPos, BaseTank * Tank)
{
	double tankSpeed = Tank->GetMaxSpeed();
	if (isArriveOn)
	{
		tankSpeed = speed;
	}
	Vector2D VectorToTarget = Vec2DNormalize(targetPos - Tank->GetCentrePosition()) * tankSpeed;
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
	Vector2D AvoidLeft(0, 0);
	Vector2D AvoidLeft2(0, 0);
	Vector2D AvoidRight(0, 0);
	Vector2D AvoidRight2(0, 0);
	std::vector<GameObject*>::const_iterator cur0b = ObstacleM.begin();
	Vector2D PosLeft = (Tank->GetCentrePosition() - (Tank->GetSide() * 10));
	Vector2D PosLeft2 = (Tank->GetCentrePosition() - (Tank->GetSide() * 30));
	Vector2D PosRight = (Tank->GetCentrePosition() + (Tank->GetSide() * 10));
	Vector2D PosRight2 = (Tank->GetCentrePosition() + (Tank->GetSide() * 30));
	Vector2D AheadCheckLeft = PosLeft + Vec2DNormalize(Tank->GetVelocity()) * AvoidCheckDistance;
	Vector2D AheadCheckLeft2 = PosLeft + Vec2DNormalize(Tank->GetVelocity()) * AvoidCheckDistance;
	Vector2D AheadCheckRight = PosRight + Vec2DNormalize(Tank->GetVelocity()) * AvoidCheckDistance;
	Vector2D AheadCheckRight2 = PosRight + Vec2DNormalize(Tank->GetVelocity()) * AvoidCheckDistance;

	for (int i = 0; i < ObstacleM.size(); i++)
	{

		if (Collisions::Instance()->PointInBox(AheadCheckLeft, ObstacleM[i]->GetAdjustedBoundingBox()))
		{
			AvoidLeft = AheadCheckLeft - ObstacleM[i]->GetCentralPosition();
			AvoidLeft = Vec2DNormalize(AvoidLeft) * 2500;
		}
		if (Collisions::Instance()->PointInBox(AheadCheckLeft2, ObstacleM[i]->GetAdjustedBoundingBox()))
		{
			AvoidLeft2 = AheadCheckLeft2 - ObstacleM[i]->GetCentralPosition();
			AvoidLeft2 = Vec2DNormalize(AvoidLeft2) * 2000;
		}
		if (Collisions::Instance()->PointInBox(AheadCheckRight, ObstacleM[i]->GetAdjustedBoundingBox()))
		{
			AvoidRight = AheadCheckRight - ObstacleM[i]->GetCentralPosition();
			AvoidRight = Vec2DNormalize(AvoidRight) * 2500;
		}
		if (Collisions::Instance()->PointInBox(AheadCheckRight2, ObstacleM[i]->GetAdjustedBoundingBox()))
		{
			AvoidRight2 = AheadCheckRight2 - ObstacleM[i]->GetCentralPosition();
			AvoidRight2 = Vec2DNormalize(AvoidRight2) * 2000;
		}
		AvoidForce += AvoidRight + AvoidLeft + AvoidRight2 + AvoidLeft2;
	}
	return AvoidForce;

}

Vector2D SteeringP016671e::ArriveAtMouse(float deltaTime, Vector2D cursorPos, BaseTank * Tank)
{
	DoAllowRotate();
	Vector2D click = cursorPos - Tank->GetCentrePosition();
	double distance = click.Length();
	const double tweaker = 0.3;
	if (distance > 50)
	{
		speed = distance / deceleration;
		speed = min(speed, Tank->GetMaxSpeed());
		Vector2D force = Seek(cursorPos, Tank);
		
		if (distance < 5)
		{
			DontAllowRotate();
		}
		return force;
	}
	if (distance < 50)
	{
		speed = distance / (deceleration * 1.5);
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

Vector2D SteeringP016671e::FollowPath(float deltaTime, Vector2D destPos, BaseTank * Tank)
{
	Vector2D force = Seek(destPos, Tank);
	return force;
}

void SteeringP016671e::AStarOff()
{
	isAStarOn = false;
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
	if (isAStarOn)
	{
		combinedForce += (FollowPath(deltaTime, cursorPos, Tank) * 3);
	}
	return combinedForce;
}

SteeringP016671e::~SteeringP016671e()
{

}
#include "SteeringP016671e.h"

SteeringP016671e::SteeringP016671e()
{

}
Vector2D SteeringP016671e::Seek(Vector2D targetPos, Vector2D curPos, Vector2D velocity, double maxSpeed)
{
	Vector2D VectorToTarget = Vec2DNormalize(targetPos - curPos) * maxSpeed;
	return (VectorToTarget - velocity);
}
SteeringP016671e::~SteeringP016671e()
{

}
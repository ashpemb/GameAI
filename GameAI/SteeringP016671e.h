#include "Commons.h"

enum TANKSTATE {
	SEEK,
	ARRIVE,
	FLEE,
	EVADE,
	PURSUIT,
	SEEKTOMOUSE,
	FLEEFROMMOUSE,

};

class SteeringP016671e
{
public:
	SteeringP016671e();
	void SeekOn(){ tankState = SEEK; }
	void FleeOn(){ tankState = FLEE; }
	void SeekMouseOn(){ tankState = SEEKTOMOUSE; }
	void FleeMouse(){ tankState = FLEEFROMMOUSE; }
	void ArriveOn(){ tankState = ARRIVE; }
	void PursuitOn(){ tankState = PURSUIT; }
	bool ObsAvoidOn(){ return isObsAvoidOn; }
	int GetState(){ return tankState; }
	bool GetAllowRotate(){ return allowRotate; }
	void DoAllowRotate(){ allowRotate = true; }
	void DontAllowRotate(){ allowRotate = false; }
	Vector2D GetForce(){ return combinedForce; }
	Vector2D CalculateForce(float deltaTime, Vector2D cursorPos, Vector2D tankPos, Vector2D velocity, double maxSpeed);
	Vector2D Seek(Vector2D targetPos, Vector2D tankPos, Vector2D velocity, double maxSpeed);
	Vector2D SeekToMouse(float deltaTime, Vector2D cursorPos, Vector2D tankPos, Vector2D velocity, double maxSpeed);
	Vector2D FleeFromMouse(float deltaTime, Vector2D cursorPos, Vector2D tankPos, Vector2D velocity, double maxSpeed);
	/*Vector2D ObstacleAvoid(float deltaTime, Vector2D cursorPos, Vector2D tankPos, Vector2D velocity, double maxSpeed);*/
	Vector2D ArriveAtMouse(float deltaTime, Vector2D cursorPos, Vector2D tankPos, Vector2D velocity, double maxSpeed);
	Vector2D PursuitMouse(float deltaTime, Vector2D cursorPos, Vector2D tankPos, Vector2D velocity, double maxSpeed);
	~SteeringP016671e();
private:
	int tankState;
	Vector2D combinedForce;
	double fleeRadius;
	double arriveRadius;
	double deceleration = 4;
	bool isObsAvoidOn;
	bool allowRotate;
};
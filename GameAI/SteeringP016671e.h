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
	int GetState(){ return tankState; }
	Vector2D GetForce(){ return combinedForce; }
	Vector2D CalculateForce(float deltaTime, Vector2D cursorPos, Vector2D tankPos, Vector2D velocity, double maxSpeed);
	Vector2D Seek(Vector2D targetPos, Vector2D tankPos, Vector2D velocity, double maxSpeed);
	Vector2D SeekToMouse(float deltaTime, Vector2D cursorPos, Vector2D tankPos, Vector2D velocity, double maxSpeed);
	Vector2D FleeFromMouse(float deltaTime, Vector2D cursorPos, Vector2D tankPos, Vector2D velocity, double maxSpeed);
	~SteeringP016671e();
private:
	int tankState;
	Vector2D combinedForce;
};
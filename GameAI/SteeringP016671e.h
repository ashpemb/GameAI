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
	void SeekOn(){ isSeekOn = true; }
	void FleeOn(){ isFleeOn = true; }
	void ArriveOn(){ isArriveOn = true; }
	void PursuitOn(){ isPursuitOn = true; }
	bool ObsAvoidOn(){ isObsAvoidOn = true; }
	bool GetSeek(){ return isSeekOn; }
	bool GetFlee(){ return isFleeOn; }
	bool GetArrive(){ return isArriveOn; }
	bool GetPursuit(){ return isPursuitOn; }
	bool GetObsAvoid(){ return isObsAvoidOn; }
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
	double deceleration = 2;
	bool isObsAvoidOn = false;
	bool isFleeOn = false;
	bool isSeekOn = false;
	bool isArriveOn = false;
	bool isPursuitOn = false;
	bool allowRotate;
};
#include "Commons.h"

enum TANKSTATE {
	SEEK,
	ARRIVE,
	FLEE,
	EVADE,
	PURSUIT,

};

class SteeringP016671e
{
public:
	SteeringP016671e();
	void SeekOn(){ tankState = SEEK; }
	void FleeOn(){ tankState = FLEE; }
	int GetState(){ return tankState; }
	Vector2D Seek(Vector2D targetPos, Vector2D tankPos, Vector2D velocity, double maxSpeed);
	~SteeringP016671e();
private:
	int tankState;
};
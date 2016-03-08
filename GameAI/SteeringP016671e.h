#include "Commons.h"
#include "ObstacleManager.h"
#include <SDL.h>
#include <vector>
#include "BaseTank.h"
#include "Collisions.h"

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
	void SeekOff(){ isSeekOn = false; }
	void FleeOn(){ isFleeOn = true; }
	void ArriveOn(){ isArriveOn = true; }
	void ArriveOff(){ isArriveOn = false; }
	void PursuitOn(){ isPursuitOn = true; }
	void ObsAvoidOn(){ isObsAvoidOn = true; }
	void AStarOn(){ isAStarOn = true; }
	void Pathoff(){ pathfindingTurnedOff = false; }
	void Pathon(){ pathfindingTurnedOff = true; }
	void AStarOff();
	bool GetSeek(){ return isSeekOn; }
	bool GetFlee(){ return isFleeOn; }
	bool GetArrive(){ return isArriveOn; }
	bool GetPursuit(){ return isPursuitOn; }
	bool GetObsAvoid(){ return isObsAvoidOn; }
	bool GetAStar(){ return isAStarOn; }
	bool GetPathOff(){ return pathfindingTurnedOff; }
	bool GetAllowRotate(){ return allowRotate; }
	void DoAllowRotate(){ allowRotate = true; }
	void DontAllowRotate(){ allowRotate = false; }
	void SetLastPos(Vector2D pos){ lastPos = pos; }
	Vector2D GetForce(){ return combinedForce; }
	Vector2D CalculateForce(float deltaTime, Vector2D cursorPos, BaseTank * Tank);
	Vector2D Seek(Vector2D targetPos, BaseTank * Tank);
	Vector2D SeekToMouse(float deltaTime, Vector2D cursorPos, BaseTank * Tank );
	Vector2D FleeFromMouse(float deltaTime, Vector2D cursorPos, BaseTank * Tank);
	Vector2D ObstacleAvoid(float deltaTime, Vector2D cursorPos, BaseTank * Tank, const std::vector<GameObject*>& ObstacleM);
	Vector2D ArriveAtMouse(float deltaTime, Vector2D cursorPos, BaseTank * Tank);
	Vector2D PursuitMouse(float deltaTime, Vector2D cursorPos, BaseTank * Tank);
	Vector2D FollowPath(float deltaTime, Vector2D destPos, BaseTank * Tank);
	~SteeringP016671e();
private:
	int tankState;
	Vector2D combinedForce;
	Vector2D lastPos;
	double speed;
	double fleeRadius;
	double arriveRadius;
	double deceleration = 2;
	double AvoidCheckDistance = 50;
	bool isObsAvoidOn = false;
	bool isFleeOn = false;
	bool isSeekOn = false;
	bool isArriveOn = false;
	bool isPursuitOn = false;
	bool isAStarOn = false;
	bool pathfindingTurnedOff = false;
	bool allowRotate;
};
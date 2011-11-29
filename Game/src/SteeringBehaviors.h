#ifndef STEERINGBEHAVIORS_H
#define STEERINGBEHAVIORS_H
#include "Ogre.h"
#include "Character.h"
class SteeringBehaviors
{
private:
	Character* mCharacter;		//Parent
	Character* mTargetAgent1;	//Target

	bool seekOn;
	bool seekCharacterSet;
	bool arriveOn;
	bool pursuitOn;

	Ogre::Vector3	mSteeringForce;
	Ogre::Real		mDistToTarget;
	Ogre::Vector3	mTarget;

	Ogre::Vector3	Seek(Ogre::Vector3);
	Ogre::Vector3	Arrive(Ogre::Vector3);
	Ogre::Vector3	Pursuit(Character* evader);
	
public:
	SteeringBehaviors(Character* agent);
	~SteeringBehaviors();

	Ogre::Vector3 Calculate();

	void SetTarget(const Ogre::Vector3 t){mTarget = t;}

	void SeekOn(){seekOn = true;seekCharacterSet = false;}
	void SeekOn(Ogre::Vector3 target){mTarget = target; seekOn = true;seekCharacterSet = false;}
	void SeekOn(Character* target)
	{
		mTargetAgent1 = target;
		seekCharacterSet = true;
		seekOn = true;
		mTarget = target->GetSceneNode()->getPosition();
	}
	void ArriveOn(){arriveOn = true;}
	void PursuitOn(Character* v){pursuitOn = true; mTargetAgent1 = v;}

	void SeekOff()  {seekOn = false;}
	void ArriveOff(){arriveOn = false;}
	void PursuitOff(){pursuitOn = false;}

	Ogre::Real CalculateDistanceToTarget();

	//getters
	Ogre::Real GetDistToTarget(){ return mDistToTarget; }

	//setters
	void SetDistToTarget(Ogre::Real value){ mDistToTarget = value; } 
protected:
	void Initialize();
};

#endif




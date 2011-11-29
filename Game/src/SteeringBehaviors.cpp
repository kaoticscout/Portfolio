#include "SteeringBehaviors.h"
SteeringBehaviors::SteeringBehaviors(Character* agent)
{
	Initialize();
	mCharacter = agent;
	mSteeringForce = Ogre::Vector3::ZERO;
	SetDistToTarget(0);
}

SteeringBehaviors::~SteeringBehaviors()
{
}

void SteeringBehaviors::Initialize()
{
	
	seekOn = false;
	arriveOn = false;
	pursuitOn = false;
	mSteeringForce = Ogre::Vector3::ZERO;

}
Ogre::Vector3 SteeringBehaviors::Pursuit(Character* evader)
{
	Ogre::Vector3 ToEvader = evader->GetSceneNode()->getPosition() - mCharacter->GetSceneNode()->getPosition();
	double RelativeHeading = mCharacter->GetHeading().dotProduct(evader->GetHeading());

	//if evader is in front and moving forward
	if((ToEvader.dotProduct(mCharacter->GetHeading())) > 0 && (RelativeHeading < -0.95))
		return Seek(evader->GetSceneNode()->getPosition());

	//not ahead so look forward
	double LookAheadTime = ToEvader.length() / (mCharacter->GetMaxSpeed() + evader->GetMaxSpeed());
	Ogre::Vector3 currentVelocity = evader->GetVelocity();
	Ogre::Real currentSpeed = currentVelocity.normalise();
	Ogre::Vector3 desiredVelocity =  Seek(evader->GetSceneNode()->getPosition() + evader->GetHeading() * currentSpeed * LookAheadTime);
	desiredVelocity.y = 0;
	return desiredVelocity;

}
Ogre::Vector3 SteeringBehaviors::Seek(Ogre::Vector3 seekTarget)
{
	Ogre::Vector3 DesiredVelocity = Ogre::Vector3(seekTarget - mCharacter->GetSceneNode()->getPosition());
	SetDistToTarget(DesiredVelocity.normalise());
	
	DesiredVelocity = DesiredVelocity * mCharacter->GetMaxSpeed();
	DesiredVelocity -= mCharacter->GetVelocity();
	return DesiredVelocity;
}


Ogre::Real SteeringBehaviors::CalculateDistanceToTarget()
{
	Ogre::Vector3 temp = Ogre::Vector3(mTarget - mCharacter->GetSceneNode()->getPosition());
	temp.y = 0;
	SetDistToTarget(temp.normalise());
	return GetDistToTarget();
}

Ogre::Vector3 SteeringBehaviors::Calculate()
{
	if(seekOn == true)
		mSteeringForce = Seek(mTarget);

	if(pursuitOn == true)
		mSteeringForce = Pursuit(mTargetAgent1);
	return mSteeringForce;

}
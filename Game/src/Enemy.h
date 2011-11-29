#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include "CON_EnemyConstants.h"
#include "SteeringBehaviors.h"

class Enemy :
	public Character
{
protected:
	SteeringBehaviors*		mSteering;
	int						mAttackDamage;
	int						mAttackSpeed;
	int						mLastAttackTime;
	float					mAttackRange;
	
	void HandleMovement(){}

public:
	Enemy::Enemy()
		: Character(CT_ENEMY)
	{
	}

	void Collision(CollisionObject* other){}
	void Initialize(){}
	void Startup(){}
	void Shutdown(){}
	bool Update(DWORD timeSinceLastFrame){ return true; }
	float GetAttackRange(){ return mAttackRange; }
};
#endif
#include "GameEngine.h"
#include "CollisionManager.h"
#include "CON_CollisionTypes.h"
#include "CON_GameConstants.h"

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Startup()
{
}

void CollisionManager::Shutdown()
{
}


bool CollisionManager::Update(DWORD timeSinceLastFramet)
{
	return true;
}

void CollisionManager::AddCollisionObject(Character* obj)
{
	if(obj->GetCollisionType() == CT_ENEMY)
		mCollisionObjects.push_back(obj);
	else
		mCollisionPlayer = obj;
}


void CollisionManager::RemoveCollisionObject(Character* obj)
{
	if(obj->GetCollisionType() == CT_ENEMY)
	{
		bool found = false;
		std::vector<Character*>::iterator removeObj;
		for(std::vector<Character*>::iterator it = mCollisionObjects.begin(); it != mCollisionObjects.end(); it++)
		{
			if(*it == obj)
			{
				found = true;
				removeObj = it;
				break;
			}
		}
		
		if(found)
			mCollisionObjects.erase(removeObj);
	}
}

//TODO: The sphere should be created ahead of time, not inside this method
//std::vector<Character*> CollisionManager::CheckCollisions(Ogre::Sphere* collisionObj)
std::vector<Character*> CollisionManager::CheckCollisions(Character* obj, int attackType)
{
	std::vector<Character*> collisions;
	
	Ogre::Real attack = 10.0f;
	Ogre::Vector3 heading = obj->GetHeading();
	Ogre::Vector3 desiredLocation = obj->GetSceneNode()->getPosition() + (heading * attack/2);
	Ogre::Sphere attackRange = Ogre::Sphere(desiredLocation, attack);

	//check all collision objects to see if player colides	
	for(int i = 0; i < mCollisionObjects.size(); i++)
	{
		if(attackRange.intersects(mCollisionObjects[i]->GetEntity()->getWorldBoundingBox()))
		{
			if(attackType == 1)
			{
				collisions.push_back(mCollisionObjects[i]);
				break;
			}
			else
			{
				collisions.push_back(mCollisionObjects[i]);
			}
		}
	}

	return collisions;
}

Character* CollisionManager::CheckCollisions(Character* obj)
{
	Ogre::Vector3 heading = obj->GetHeading();
	heading *= .5;//lol
	Ogre::Sphere* attackRange = new Ogre::Sphere(obj->GetSceneNode()->getPosition(), (Ogre::Real)(2*ENEMY_CLOSE_DISTANCE));

	//check if player collides with enemies
	//only attack 1 enemy maximum
	if(obj->GetCollisionType() == CT_PLAYER)
	{
		for(int i = 0; i < mCollisionObjects.size(); i++)
		{
			if(attackRange->intersects(mCollisionObjects[i]->GetBoundingBox()))
			{
				return mCollisionObjects[i];
				break;
			}
		}
	}
	else if(obj->GetCollisionType() == CT_ENEMY)
	{
		if(attackRange->intersects(GAMEENGINE.GetPlayer()->GetBoundingBox()))
			return GAMEENGINE.GetPlayer();
	}

	return NULL;
}


#include "CollisionObject.h"
#include "GameEngine.h"

CollisionObject::CollisionObject(int type) :
	mCollisionType(type)
{
	Startup();
}

CollisionObject::~CollisionObject()
{
	Shutdown();
}

void CollisionObject::Startup()
{
}

void CollisionObject::Shutdown()
{
	UnregisterCollisionObject();
}

void CollisionObject::RegisterCollisionObject()
{
	GAMEENGINE.GetCollisionManager()->AddCollisionObject(static_cast<Character*>(this));
}

void CollisionObject::UnregisterCollisionObject()
{
	GAMEENGINE.GetCollisionManager()->RemoveCollisionObject(static_cast<Character*>(this));
}
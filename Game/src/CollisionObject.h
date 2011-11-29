#ifndef COLLISIONOBJECT_H
#define COLLISIONOBJECT_H

#include "Ogre.h"
#include "GameComponent.h"
#include "CON_CollisionTypes.h"
#include "GraphicsEngine.h"

class CollisionObject 
	: public GameComponent
{
protected:
	int		mCollisionType;
public:
	CollisionObject(int type);
	virtual ~CollisionObject();

	virtual void Initialize(){}
	void Startup();
	void Shutdown();
	virtual bool Update(DWORD timeSinceLastFrame){ return true; }
	virtual void Collision(CollisionObject* other) { }

	inline int GetCollisionType() const {return mCollisionType;}
	void RegisterCollisionObject();
	void UnregisterCollisionObject();
};

#endif
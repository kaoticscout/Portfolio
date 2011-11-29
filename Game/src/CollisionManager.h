#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H	

#include "CollisionObject.h"
#include "GameComponent.h"
#include <vector>

#include "Character.h"

typedef std::vector<Character*> CollisionObjectsList;

class CollisionManager
	: public GameComponent
{
private:
	CollisionObjectsList mCollisionObjects;
	CollisionObject* mCollisionPlayer;
public:
	CollisionManager();
	~CollisionManager();
	void Startup();
	void Shutdown();
	bool Update(DWORD timeSinceLastFrame);

	void AddCollisionObject(Character* obj);
	void RemoveCollisionObject(Character* obj);
	Character* CheckCollisions(Character* obj);
	std::vector<Character*> CheckCollisions(Character* obj, int attackType);
};
#endif

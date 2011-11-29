#ifndef GUARDIAN_H
#define GUARDIAN_H

#include "Enemy.h"

class Guardian:
	public Enemy
{

protected:
	void HandleMovement();
	Ogre::SceneNode* tempNode;
public:
	Guardian();
	Guardian(Ogre::String name, Ogre::String mesh, Ogre::String texture);

	void Initialize();
	void Startup();
	void Shutdown();
	bool Update(DWORD timeSinceLastFrame);
	void PursuePlayer();
	void AutoAttack();
	void Wander();
};
#endif

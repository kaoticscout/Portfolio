#ifndef MINION_H
#define MINION_H

#include "Enemy.h"

class Minion:
	public Enemy
{
protected:
	void HandleMovement();
public:
	Minion();
	Minion(Ogre::String name, Ogre::String mesh, Ogre::String texture);
	
	void Initialize();
	void Startup();
	void Shutdown();
	bool Update(DWORD timeSinceLastFrame);
	void AutoAttack();
	void PursuePlayer();
};
#endif



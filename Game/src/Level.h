#ifndef LEVEL_H
#define LEVEL_H

#include "GameComponent.h"
#include "Hut.h"
#include "Minion.h"
#include "Guardian.h"

class Level
	: public GameComponent
{
private:
	int					  mLevelId;
	int					  mMaxNumMinions;

	Ogre::SceneNode*	  mLevelNode;
	Ogre::Entity*		  mGround;
	std::vector<Hut*>	  mLevelHuts;
	std::vector<Enemy*>	  mLevelEnemies;

	Ogre::Vector3 GenerateHutLocation(int maxXPos, int maxYPos);

public:
	Level();
	bool Update(DWORD timeSinceLastFrame);
	void Initialize();
	void SetupLevel(Hut* hutTemplate, Guardian* guardianT, Ogre::String levelTexture, int numHuts);
	void Startup();
	void Shutdown();
	
	void SetLevelId(int newId) { mLevelId = newId; }

	int GetLevelId()   const { return mLevelId; }
	Ogre::Entity* GetLevelMesh() const { return mGround; }
	Ogre::SceneNode* GetSceneNode() const { return mLevelNode; }
	std::vector<Hut*> GetHutList()         { return mLevelHuts; }
	std::vector<Enemy*>* GetEnemyList()       { return &mLevelEnemies; }

	void SetMaxNumMinions(int newMax){ mMaxNumMinions = newMax; }
};

#endif


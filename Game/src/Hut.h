#ifndef HUT_H
#define HUT_H

#include "CON_HutConstants.h"
#include "GameComponent.h"
#include "Guardian.h"
#include "Minion.h"
#include "StateManager.h"
#include "OgreString.h"
#include <list>

class Hut
	: public GameComponent
{

private:
	std::vector<Guardian*>	mListOfGuardians;

	Ogre::SceneNode*		mHutNode;
	Ogre::Entity*			mHutEntity;
	Minion* 				mMinionTemplate;

	int						mSpawnRadius;
	int						mGuardianSpawnAmount;
	Ogre::Real				mAggroRadius;
	Ogre::Real				mNextSpawnTime;
	Ogre::Real				mLastSpawnTime;
	Ogre::Real				mTimeBetweenSpawns;

	Ogre::Sphere*			mAggroSphere;
	bool					mIsActive;

	Ogre::String			mMeshLocation;
	Ogre::String			mTextureLocation;

	void					SpawnMinion();
	Ogre::Vector3			GenerateEnemyLocation(bool isGuardian);

	bool					CheckAggro();
	bool					CheckDeath();

public:
	Hut(); //Default
	Hut(Ogre::String mesh, Ogre::String texture, int spawnRadius, //Dynamic Hut object
		Ogre::Real aggroRadius, Ogre::Real timeBetweenSpawns, int guardianSpawnAmount = 0);
	Hut(Ogre::String mesh, Ogre::String texture, Minion* minionT, int spawnRadius, //Template Hut object (for level generation)				
		Ogre::Real aggroRadius, Ogre::Real timeBetweenSpawns, int guardianSpawnAmount = 0);

	void				GenerateGuardians(int spawnNum, Guardian* guardianT);

	void				Initialize();
	void				Startup();
	void				Shutdown();
	virtual bool		Update(DWORD timeSinceLastFrame);
	
	//getters
	bool				IsActive()			  const { return mIsActive; }
	Ogre::Real			GetAggroRadius()	  const { return mAggroRadius; }
	Ogre::Entity*		GetEntity()			  const { return mHutEntity; }
	int					GetGuardianSpawnAmt() const { return mGuardianSpawnAmount; }
	Ogre::String		GetMesh()			  const { return mMeshLocation; }
	Minion*				GetMinionTemplate()   const { return mMinionTemplate; }
	Ogre::SceneNode*	GetSceneNode()		  const { return mHutNode; }
	int					GetSpawnRadius()	  const { return mSpawnRadius; }
	Ogre::String		GetTexture()		  const { return mTextureLocation; }
	Ogre::Real			GetTimeBetweenSpawns()const { return mTimeBetweenSpawns; }

	//setters
	void SetAggroRadius(Ogre::Real value){ mAggroRadius = value; }
	void SetEntity(Ogre::Entity* entity){ mHutEntity = entity; }
	void SetSpawnRadius(int value){ mSpawnRadius = value; }
	void SetTimeBetweenSpawns(Ogre::Real timeBetweenSpawns){ mTimeBetweenSpawns = timeBetweenSpawns; }
	void SetTexture(Ogre::String textureLoc){ mHutEntity->setMaterialName(textureLoc); }
};

#endif

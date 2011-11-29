#include "Level.h"
#include "GameEngine.h"
#include "GraphicsEngine.h"

typedef std::vector<Hut*>::iterator hutIterator;
typedef std::vector<Enemy*>::iterator enemyIterator;

Level::Level()
{
	Initialize();
}

void Level::Initialize()
{
	GRAPHICSENGINE.GetSceneManager()->setAmbientLight(Ogre::ColourValue(0.4, 0.4, 0.4));
    GRAPHICSENGINE.GetSceneManager()->setShadowTechnique(Ogre::SHADOWTYPE_NONE);

	mGround  = GRAPHICSENGINE.GetSceneManager()->createEntity("ground");
	mLevelNode = GRAPHICSENGINE.GetSceneManager()->createSceneNode();
	
	mLevelNode->pitch(Ogre::Degree(-90));
	mGround->setCastShadows(false);

	GRAPHICSENGINE.GetSceneManager()->getRootSceneNode()->addChild(mLevelNode);
}

bool Level::Update(DWORD timeSinceLastFrame)
{
	//update huts
	for(hutIterator it = mLevelHuts.begin(); it != mLevelHuts.end(); ++it)
	{
		(*it)->Update(timeSinceLastFrame);
	}
	//update enemies
	for(enemyIterator it = mLevelEnemies.begin(); it != mLevelEnemies.end(); ++it)
	{
		Enemy* currentEnemy = (*it);
		if(currentEnemy != NULL)
		{
			currentEnemy->Update(timeSinceLastFrame);
		}
		(*it)->Update(timeSinceLastFrame);
	}

	//delete inactive huts
	for(int i = mLevelHuts.size() - 1; i >= 0; --i)
	{
		Hut* currentHut = mLevelHuts[i];
		if(currentHut->IsActive() == false)
		{
			delete currentHut;
			mLevelHuts.erase(mLevelHuts.begin() + i);
		}
	}

	//delete inactive enemies
	std::vector<Enemy*> tempList;
	for(int i = mLevelEnemies.size() - 1; i >= 0; --i)
	{
		Enemy* currentEnemy = mLevelEnemies[i];
		if(currentEnemy->IsActive() == false)
		{
			delete currentEnemy;
			mLevelEnemies.erase(mLevelEnemies.begin() + i);
		}
	}

	return true;
}

void Level::SetupLevel(Hut* hutTemplate, Guardian* guardianTemplate, Ogre::String levelTexture, int numHuts)
{
	mGround->setMaterialName(levelTexture);

	Hut* newHut;
	for(int i = 0; i < numHuts; ++i)
	{
		newHut = new Hut(hutTemplate->GetMesh(),
			hutTemplate->GetTexture(), hutTemplate->GetMinionTemplate(), hutTemplate->GetAggroRadius(), 
			hutTemplate->GetSpawnRadius(), hutTemplate->GetTimeBetweenSpawns());

		newHut->GetSceneNode()->setPosition(GenerateHutLocation(300, 300)); 
		newHut->GenerateGuardians(hutTemplate->GetGuardianSpawnAmt(), guardianTemplate);

		mLevelHuts.push_back(newHut);
	}
}

void Level::Startup()
{
	mLevelNode->attachObject(mGround);

	//start up all huts in the level
	for(hutIterator it = mLevelHuts.begin(); it != mLevelHuts.end(); ++it)
	{
		(*it)->Startup();
	}
}

void Level::Shutdown()
{
	//delete all huts
	for(hutIterator it = mLevelHuts.begin(); it != mLevelHuts.end(); ++it)
	{
		Hut* currentHut = (*it);
		if(currentHut->IsActive())
		{
			currentHut->Shutdown();
		}
		delete currentHut;
	}
	mLevelHuts.clear();

	//delete all enemies
	for(enemyIterator it = mLevelEnemies.begin(); it != mLevelEnemies.end(); ++it)
	{
		Enemy* currentEnemy = (*it);
		if(currentEnemy->IsActive())
		{
			currentEnemy->Shutdown();
		}
		delete currentEnemy;
	}
	mLevelEnemies.clear();

	mLevelNode->detachAllObjects();
}

Ogre::Vector3 Level::GenerateHutLocation(int maxXPos, int maxYPos)
{
	static std::vector<Ogre::Vector2> quadrants; //Persistant list that tracks available quadrants to spawn into
	
	//If there are no specific quadrants to add to then all 
	//quadrants are valid so add all quadrants.
	if(quadrants.size() <= 0) 
	{
		//Break plane into 4 quadrants
		quadrants.push_back(Ogre::Vector2( 1 ,  1));     //Quadrant 1 +x +y
		quadrants.push_back(Ogre::Vector2( 1 , -1));     //Quadrant 2 -x +y
		quadrants.push_back(Ogre::Vector2(-1 , -1));     //Quadrant 3 -x -y
		quadrants.push_back(Ogre::Vector2(-1 ,  1));     //Quadrant 3 +x -y
	}

	int targetQuadrant = (rand() % quadrants.size());

	Ogre::Vector2 quadToSpawnTo = quadrants.at(targetQuadrant);
	quadrants.erase(quadrants.begin() + targetQuadrant);

	int randomX = ((rand() % maxXPos)  + WAYPOINT_BUFFER)  * quadToSpawnTo.x;
	int randomZ = ((rand() % maxYPos)  + WAYPOINT_BUFFER)  * quadToSpawnTo.y;
	
	return Ogre::Vector3(randomX, 0,  randomZ);
}

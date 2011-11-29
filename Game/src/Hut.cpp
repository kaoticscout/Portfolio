#include "GraphicsEngine.h"
#include "GameEngine.h"
#include "Hut.h"

Hut::Hut()
{	
	mMinionTemplate = new Minion();

	//mMeshLocation = DEFAULT_HUT_MESH;
	//mTextureLocation =	DEFAULT_HUT_TEXTURE;

	mMeshLocation = "Hut.mesh";
	mTextureLocation = "Rockwall";

	mSpawnRadius = DEFAULT_SPAWN_RADIUS;
	mAggroRadius = DEFAULT_AGGRO_RADIUS;
	mTimeBetweenSpawns = DEFAULT_TIME_BETWEEN_SPAWNS;
	mGuardianSpawnAmount = DEFAULT_GUARDIAN_SPAWN_AMOUNT;

	Initialize();
}

Hut::Hut(Ogre::String mesh, //Constructor for hut object
	Ogre::String texture, 
	int spawnRadius,
	Ogre::Real aggroRadius,
	Ogre::Real timeBetweenSpawns,
	int guardianSpawnAmount)
{
	mMeshLocation = mesh;
	mTextureLocation = texture;
	mSpawnRadius = spawnRadius;
	mAggroRadius = aggroRadius;
	mTimeBetweenSpawns = timeBetweenSpawns;
	mGuardianSpawnAmount = guardianSpawnAmount;

	Initialize();
}

Hut::Hut(Ogre::String mesh, //Constructor for a hut template
	Ogre::String texture,
	Minion* minionTemplate,
	int	spawnRadius,
	Ogre::Real aggroRadius, 
	Ogre::Real timeBetweenSpawns,
	int guardianSpawnAmount)
{
	mMinionTemplate = minionTemplate;
	
	mMeshLocation = mesh;
	mTextureLocation = texture;
	mSpawnRadius = spawnRadius;
	mAggroRadius = aggroRadius;
	mTimeBetweenSpawns = timeBetweenSpawns;
	mGuardianSpawnAmount = guardianSpawnAmount;

	Initialize();
}

void Hut::Initialize()
{	
	mIsActive = true;
	mHutEntity = GRAPHICSENGINE.GetSceneManager()->createEntity(mMeshLocation);
	mHutEntity->setCastShadows(true);
	
	mHutNode = GRAPHICSENGINE.GetSceneManager()->createSceneNode();
	mHutNode->scale(0.5f, 0.3f, 0.5f);
	mHutNode->yaw(Ogre::Degree(-90));
	mAggroSphere = new Ogre::Sphere();
}

void Hut::Startup()
{
	mLastSpawnTime = 0;
	mNextSpawnTime = mLastSpawnTime + mTimeBetweenSpawns + (rand( ) % MAX_RAND_SPAWN_TIME_DIFF);

	//Attach hut to level
	if(mHutNode != NULL && mHutEntity != NULL)
	{
		mHutNode->attachObject(mHutEntity);
		GRAPHICSENGINE.GetSceneManager()->getRootSceneNode()->addChild(mHutNode);
		mAggroSphere->setRadius(mAggroRadius);
		mAggroSphere->setCenter(mHutNode->getPosition());
	}

	//Turn guardians on
	for(int i = 0; i < mListOfGuardians.size(); i++)
		mListOfGuardians[i]->Startup();
}

void Hut::Shutdown()
{
	mHutNode->detachAllObjects();
	
	delete(mHutNode);
	mHutNode = NULL;

	delete(mHutEntity);
	mHutEntity = NULL;

	for(int i = mListOfGuardians.size()-1; i >= 0 ; i--)
	{
		mListOfGuardians[i]->Shutdown();
		delete(mListOfGuardians[i]);
	}
	mListOfGuardians.clear();
}

bool Hut::Update(DWORD timeSinceLastFrame)
{
	if(!mIsActive)
		return false;
	
	//delete guardians
	std::vector<Guardian*> tempList;
	for(int i = 0; i < mListOfGuardians.size(); i++)
	{
		if(mListOfGuardians[i]->IsActive())
			tempList.push_back(mListOfGuardians[i]);
	}
	mListOfGuardians = tempList;

	if(mNextSpawnTime < GAMEENGINE.GetTotalTimeElapsed())
		SpawnMinion();

	for(int i = 0; i< mListOfGuardians.size(); i++)
		mListOfGuardians.at(i)->Update(timeSinceLastFrame);

	CheckDeath();
	CheckAggro();
	
	return true;
}

void Hut::SpawnMinion()
{	
	if( GAMEENGINE.GetLevelManager()->GetCurrentLevel()->GetEnemyList()->size() 
		< GAMEENGINE.GetLevelManager()->GetMaxNumEnemies())
	{
		Minion* minion = new Minion(mMinionTemplate->GetName(), mMinionTemplate->GetMesh(), mMinionTemplate->GetTexture());
		minion->GetSceneNode()->setPosition(GenerateEnemyLocation(false));
		minion->Startup();
		
		GAMEENGINE.GetLevelManager()->GetCurrentLevel()->GetEnemyList()->push_back(minion);
		mLastSpawnTime = GAMEENGINE.GetTotalTimeElapsed(); 
	}

	int randOffset = (rand() % MAX_RAND_SPAWN_TIME_DIFF);
	mNextSpawnTime = mLastSpawnTime + (mTimeBetweenSpawns + randOffset)*1000;
}

Vector3 Hut::GenerateEnemyLocation(bool isGuardian)
{
	Ogre::Real x, z, theta;
	int randomRadius, height;
	if(isGuardian)
	{
		randomRadius = (rand()%mSpawnRadius)+15;
		int randomThetaDegrees = (rand()%360)+1;
		theta = Ogre::Math::DegreesToRadians(randomThetaDegrees);
		height = 3;
	}
	else
	{
		randomRadius = (rand()%mSpawnRadius)+5;
		theta = Ogre::Math::DegreesToRadians(0);
		height = 1;
	}

	x = randomRadius*Ogre::Math::Cos(theta);
	z = randomRadius*Ogre::Math::Sin(theta);

	//spawn the new minion in the level
	return Ogre::Vector3(x, height, z)  + mHutNode->getPosition();
}

void Hut::GenerateGuardians(int spawnNum, Guardian* guardianT)
{
	Guardian* guardian;
	
	//Erase any guardians that may exist for this hut already and spawn a fresh batch
	if(mListOfGuardians.size() > 0)
	{
		for(int i = 0; i< mListOfGuardians.size(); i++)
		{
			guardian = mListOfGuardians.at(i);
			
			delete(guardian);
			guardian = NULL;
		}
	}

	//Spawn assigned guardians for hut
	for(int i = 0; i < spawnNum; i++)
	{
		guardian = new Guardian(guardianT->GetName(), guardianT->GetMesh(), guardianT->GetTexture());
		guardian->GetSceneNode()->setPosition(GenerateEnemyLocation(true));

		mListOfGuardians.push_back(guardian);
	}
}
bool Hut::CheckAggro()
{
	if(mAggroSphere->intersects(GAMEENGINE.GetPlayer()->GetBoundingBox()))
	{
		for(int i = 0; i< mListOfGuardians.size(); i++)
			mListOfGuardians.at(i)->GetStateManager()->SetCurrentState(PURSUE);
		return true;
	}
	return false;
}

bool Hut::CheckDeath()
{
	if(mIsActive && mListOfGuardians.size() <= 0)
	{
		Shutdown();
		mIsActive = false;
	}
	return true;
}
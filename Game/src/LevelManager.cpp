#include "LevelManager.h"
#include "GraphicsEngine.h"
#include "GameEngine.h"

LevelManager::LevelManager()
{
	Initialize();
}

LevelManager::~LevelManager()
{
}

void LevelManager::Initialize()
{
	static bool isPlaneInit = false;
	
	if(!isPlaneInit)
	{
		Ogre::Plane plane(Ogre::Vector3::UNIT_Z, 0);
		Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			plane, DEFAULT_LEVEL_WIDTH, DEFAULT_LEVEL_HEIGHT, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
		isPlaneInit = true;
	}

	GRAPHICSENGINE.GetSceneManager()->setSkyBox(true, "CloudyNoonSkyBox", 5000, true);

	mCurrentLevel = new Level();
	mCurrentLevel->SetLevelId(0);
	mLevelParser = new XmlLevelParser("Levels.xml");
}

void LevelManager::Startup()
{
}

void LevelManager::Shutdown()
{
	//Destroy the level
	mCurrentLevel->Shutdown();
	delete mCurrentLevel;
	mCurrentLevel = NULL;

	//Destroy the xml parser
	delete mLevelParser;
	mLevelParser = NULL;
}

bool LevelManager::Update(DWORD timeSinceLastFrame)
{
	//Updates huts and other level components (done strictly before updating enemies. 
	//This way new enemies spawned by the huts start updating right away)
	mCurrentLevel->Update(timeSinceLastFrame);

	//Check if player has killed all huts (Level completion)
	//load next level and reset player health and location
	if(mCurrentLevel->GetHutList().size() <= 0)
	{
		GAMEENGINE.GetPlayer()->GetSceneNode()->setPosition(0,GAMEENGINE.GetPlayer()->GetSceneNode()->getPosition().y,0);
		GAMEENGINE.GetPlayer()->ModifyHealth(GAMEENGINE.GetPlayer()->GetMaxHealth());
		LoadNextLevel();
	}

	return true;
}

int LevelManager::LoadLevel(int levelId)
{
	Ogre::String text = "Level ";

	//convert levelId to string
	std::string levelString;
	std::stringstream out;
	out << levelId;
	levelString = out.str();

	text.append(levelString);
	text.append(" starting");
	GAMEENGINE.GetGUIManager()->AddAlert(text);

	//Variables needed to generate a new level
	Hut* hutTemplate;
	Guardian* guardianTemplate;
	int numHutsToSpawn = DEFAULT_NUM_HUTS_TO_SPAWN; 
	int numMaxEnemies = DEFAULT_MAX_NUM_ENEMIES;
	Ogre::String levelTexture;

	bool levelLoaded = mLevelParser->parseLevel(levelId, &levelTexture, &hutTemplate, &guardianTemplate, &numHutsToSpawn, &numMaxEnemies);
	std::cout<<"Level Loaded: "<<levelLoaded<<std::endl;
	if(levelLoaded)
	{
		mCurrentLevel->Shutdown();
		
		mMaxNumEnemies = numMaxEnemies;
		mCurrentLevel->SetupLevel(hutTemplate, guardianTemplate, levelTexture, numHutsToSpawn);
		mCurrentLevel->SetLevelId(mCurrentLevel->GetLevelId()+1);
		
		mCurrentLevel->Startup();
	}
	else
	{
		std::cout<<"Could not load level."<<std::endl;
		return -1;
	}
	return 1;
}

int LevelManager::LoadNextLevel()
{
	std::cout<<"Trying to loading next level..."<<std::endl;
	LoadLevel(mCurrentLevel->GetLevelId()+1);
	return 1;
}
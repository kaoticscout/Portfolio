#include "XmlLevelParser.h"

XmlLevelParser::XmlLevelParser()
	:XmlParser()
{
}

XmlLevelParser::XmlLevelParser(char* xmlFileName)
	:XmlParser(xmlFileName)
{
}

XmlLevelParser::~XmlLevelParser()
{
}

bool XmlLevelParser::parseLevel(int currentLevelId, Ogre::String* levelTexture, Hut** hutTemplate, Guardian** guardianTemplate, int* numHutsToSpawn, int* maxNumEnemies)
{
	std::cout<<"Parsing level!!!"<<std::endl;

	//Grabs the xml node that contains all the information for the next level
	TiXmlElement* currentLevelNode = SearchForElement(rootNode, "Level", currentLevelId);

	if(!currentLevelNode) return false; //No more levels left to load (win condition!!!)

	TiXmlElement* hutNode;		//Root element for huts in level xml file
	TiXmlElement* enemiesNode;  //Root element for enemies in level xml file

	*levelTexture   = currentLevelNode->Attribute("Texture");
	*numHutsToSpawn = atoi(currentLevelNode->Attribute("NumHutsToSpawn"));
	*maxNumEnemies  = atoi(currentLevelNode->Attribute("MaxNumEnemies"));

	*guardianTemplate = new Guardian();
	Minion* minionTemplate   = new Minion();

	//Fetch enemy related data from the Level file
	enemiesNode = SearchForElement(currentLevelNode, "Enemies");
	ProcessEnemyData(enemiesNode, minionTemplate, *guardianTemplate);

	//Fetch hut related data from the Level file	
	hutNode = SearchForElement(currentLevelNode, "Huts");
	ProcessHutData(hutNode, hutTemplate, minionTemplate);

	return true;
}

void XmlLevelParser::ProcessEnemyData(TiXmlElement* enemyNode, Minion* minionTemplate, Guardian* guardianTemplate)
{
	if(enemyNode)
	{
		TiXmlElement* currentNode;

		currentNode = SearchForElement(enemyNode, "Minion");
		if(currentNode)
		{
			minionTemplate->SetName(currentNode->Attribute("Name"));
			minionTemplate->SetMesh(currentNode->Attribute("Mesh"));
			minionTemplate->SetTexture(currentNode->Attribute("Texture"));
			std::cout<<"Parsed minion: "<<minionTemplate->GetName()<<" texture: "<<currentNode->Attribute("Mesh")<<std::endl;
		}

		currentNode = SearchForElement(enemyNode, "Guardian");
		if(currentNode)
		{
			guardianTemplate->SetName(currentNode->Attribute("Name"));
			guardianTemplate->SetMesh(currentNode->Attribute("Mesh"));
			guardianTemplate->SetTexture(currentNode->Attribute("Texture"));
			std::cout<<"Parsed guardian: "<<guardianTemplate->GetName()<<" texture: "<<currentNode->Attribute("Mesh")<<std::endl;
		}
	}	
}

void XmlLevelParser::ProcessHutData(TiXmlElement* hutNode, Hut** hutTemplate, Minion* minionTemplate)
{
	if(hutNode)
	{
		//Grab specific values necessary for setting up the huts
		Ogre::String meshLocation = hutNode->Attribute("Mesh");
		Ogre::String textureLocation = hutNode->Attribute("Texture");
		
		int xpYeild = atoi(hutNode->Attribute("XpYeild"));
		int spawnRadius = atoi(hutNode->Attribute("SpawnRadius"));
		int guardianSpawnAmt = atoi(hutNode->Attribute("GuardianSpawnAmount"));
		
		Ogre::Real aggroRadius = (Ogre::Real)atof(hutNode->Attribute("AggroRadius"));
		Ogre::Real timeBetweenSpawns = (Ogre::Real)atof(hutNode->Attribute("TimeBetweenSpawns"));

		*hutTemplate = new Hut(meshLocation, textureLocation, minionTemplate, spawnRadius, aggroRadius, timeBetweenSpawns, guardianSpawnAmt);

		std::cout<<"Parsed hut data."<<std::endl;
	}	
	else
		*hutTemplate = new Hut();
}


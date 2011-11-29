#ifndef XML_LEVEL_PARSER_H
#define XML_LEVEL_PARSER_H

#include "XmlParser.h"
#include "Level.h"
#include "Minion.h"
#include "Guardian.h"

class XmlLevelParser
	: public XmlParser
{
private:
	void ProcessHutData(TiXmlElement* hutNode, Hut** hutTemplate, Minion* minionTemplate);
	void ProcessEnemyData(TiXmlElement* enemyNode, Minion* minionPtr, Guardian* guardianPtr);
public:
	XmlLevelParser();
	XmlLevelParser( char* fileName );
	~XmlLevelParser();

	bool parseLevel( int currentLevelId, Ogre::String* levelTexture, Hut** hutTemplate, Guardian** guardianTemplate, 
		int* numHutsToSpawn, int* maxNumEnemies);
};

#endif

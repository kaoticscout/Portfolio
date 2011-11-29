#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "CON_LevelConstants.h"
#include "XmlLevelParser.h"
#include "Level.h"
#include "Hut.h"
#include "Guardian.h"
#include "GameComponent.h"

class LevelManager 
	: public GameComponent
{

private:
	Level*				mCurrentLevel;
	XmlLevelParser*		mLevelParser;

	int					mLevelHeight;
	int					mLevelWidth;
	int					mMaxNumEnemies;
	int					mCurrentNumEnemies;
public:
	LevelManager();
	~LevelManager();

	void Initialize();
	void Startup();
	void Shutdown();
	bool Update(DWORD timeSinceLastTick);

	int	LoadLevel(int levelId);
	int	LoadNextLevel();
	
	inline void SetCurrentNumEnemies(int numEnemies){ mCurrentNumEnemies = numEnemies; }
	inline int GetCurrentNumEnemies() const { return mCurrentNumEnemies; }
	inline int GetMaxNumEnemies() const { return mMaxNumEnemies; }
	inline Level* GetCurrentLevel() const { return mCurrentLevel; }
}; 
#endif
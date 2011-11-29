#ifndef GAME_COMPONENT_H
#define GAME_COMPONENT_H

#include <Windows.h>
#include "CON_GameConstants.h"

class GameComponent
{

protected:
	bool isStarted;

public:
	GameComponent()
	{
		isStarted = false;
	}
	virtual ~GameComponent()
	{
	}

	virtual void Startup(){}
	virtual void Shutdown(){}
	virtual void Initialize(){}
	virtual bool Update(DWORD timeSinceLastFrame){ return true; }

};


#endif
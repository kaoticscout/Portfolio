#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include "GraphicsEngine.h"
#include "GameEngine.h"

void RunMainGameLoop();

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#define WIN32_LEAN_AND_MEAN
	#include "windows.h"
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
	#ifdef _DEBUG
		if (GRAPHICSENGINE.Startup(std::string("plugins_d.cfg"), std::string("ogre.cfg"), std::string("Ogre.log")))
	#else
		if (ENGINE.Startup(std::string("plugins.cfg"), std::string("ogre.cfg"), std::string("Ogre.log")))
	#endif

	//Initialize resources
	GAMEENGINE.Startup();

	//Start main game loop
	RunMainGameLoop();

	//Shut down resources
	GAMEENGINE.Shutdown();

	//End rendering loop
	GRAPHICSENGINE.Shutdown();
    
	//Removes the debugging console
	FreeConsole();
}

void RunMainGameLoop()
{
	const DWORD GAME_STARTED_TIME = GetTickCount();
	const int GAME_TICKS_PER_SECOND = 60;
    const int SKIP_TICKS = 1000 / GAME_TICKS_PER_SECOND;
    const int MAX_FRAMESKIP = 3;

    DWORD nextGameTick = GetTickCount(); //GetTickCount returns time since system started in milliseconds
	DWORD timeSinceLastFrame; //Used to track time between game logic frames
    int loops;	//Tracks how many game logic passes have been made on a single game loop pass.

	while(GAMEENGINE.GetStateManager() != NULL && GAMEENGINE.GetStateManager()->GetCurrentState() != GAME_STATE::SHUTTING_DOWN ) 
	{
        loops = 0;
		Ogre::WindowEventUtilities::messagePump(); //Allows windows to refresh properly when alt-tabbed or looking at diffrent windows at the same time

        while( GetTickCount() > nextGameTick && loops < MAX_FRAMESKIP) 
		{
			//timeSinceLastFrame = ((GetTickCount()/1000)-(GAME_STARTED_TIME/1000)) - (nextGameTick - SKIP_TICKS);
			timeSinceLastFrame = SKIP_TICKS;
			GAMEENGINE.Update(timeSinceLastFrame);

            nextGameTick += SKIP_TICKS;
            loops++;
        }
		GRAPHICSENGINE.GetRoot()->renderOneFrame(); //Render the current frame of the game
    }
}
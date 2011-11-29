#ifndef SCREEN_PLAYINGLEVEL_H
#define SCREEN_PLAYINGLEVEL_H

#include "GUIScreen.h"

class Screen_PlayingLevel
	: public GUIScreen
{
private:
	CEGUI::Window*					mMainWindow;
	CEGUI::Window*					mWindow;
	CEGUI::Window*					mHealthBarFrame;
	CEGUI::Window*					mHealthBar;
	
protected:
public:
	Screen_PlayingLevel(const CEGUI::String& type, const CEGUI::String& name, CEGUI::Window* mainWindow)
		: GUIScreen(type, name)
	{
		mMainWindow = mainWindow;
		Startup();
	}
	~Screen_PlayingLevel()
	{
		Shutdown();
	}

	void Startup();
	void Shutdown();
	void Update(DWORD timeSinceLastFrame);
};

#endif
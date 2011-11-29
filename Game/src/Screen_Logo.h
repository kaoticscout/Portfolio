#ifndef SCREEN_LOGO_H
#define SCREEN_LOGO_H

#include "GUIScreen.h"
#include "boost/timer.hpp"

class Screen_Logo	
	: public GUIScreen
{
private:
	CEGUI::Window*					mMainWindow;
	CEGUI::Window*					mWindow;
	float							mCurrentTimeRemaining;
	boost::timer*					mTimer;
	bool							mDisplaying;

	void StartGame();
public:
	Screen_Logo(const CEGUI::String& type, const CEGUI::String& name, CEGUI::Window* mainWindow)
		: GUIScreen(type, name)
	{
		mMainWindow = mainWindow;
		Startup();
	}

	virtual ~Screen_Logo()
	{
		Shutdown();
	}

	void Startup();
	void Shutdown();
	void Update(DWORD timeSinceLastFrame);
};

#endif
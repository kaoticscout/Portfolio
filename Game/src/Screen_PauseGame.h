#ifndef SCREEN_PAUSEGAME_H
#define SCREEN_PAUSEGAME_H

#include "GUIScreen.h"
#include <vector>

class Screen_PauseGame
	: public GUIScreen
{
private:
	CEGUI::Window*					mMainWindow;
	CEGUI::Window*					mWindow;
	CEGUI::PushButton*				mExitGameButton;
	CEGUI::PushButton*				mLogoutButton;
protected:
public:
	Screen_PauseGame(const CEGUI::String& type, const CEGUI::String& name, CEGUI::Window* mainWindow)
		: GUIScreen(type, name)
	{
		mMainWindow = mainWindow;
		Startup();
	}

	virtual ~Screen_PauseGame()
	{
		Shutdown();
	}

	void Startup();
	void Shutdown();
	void Update(DWORD timeSinceLastFrame);
	
	bool LogoutKeyPressed(const CEGUI::EventArgs&);
	bool ExitGameKeyPressed(const CEGUI::EventArgs&);
};

#endif

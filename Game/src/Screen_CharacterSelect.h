#ifndef SCREEN_CHARACTERSELECT_H
#define SCREEN_CHARACTERSELECT_H

#include "GUIScreen.h"
#include <vector>

class Screen_CharacterSelect
	: public GUIScreen
{
private:
	CEGUI::Window*					mMainWindow;
	CEGUI::Window*					mWindow;
	
	//char window
	CEGUI::PushButton*				mCharacterButton;
	CEGUI::Window*					mCharacterWindow;
	CEGUI::Window*					mCharacterIconWindow;
	CEGUI::Window*					mCharacterText;
	
	CEGUI::PushButton*				mBackButton;
	CEGUI::PushButton*				mLoginButton;
protected:
public:
	Screen_CharacterSelect(const CEGUI::String& type, const CEGUI::String& name, CEGUI::Window* mainWindow)
		: GUIScreen(type, name)
	{
		mMainWindow = mainWindow;
		Startup();
	}

	virtual ~Screen_CharacterSelect()
	{
		Shutdown();
	}

	void Startup();
	void Shutdown();
	void Update(DWORD timeSinceLastFrame);

	void loadCharacterScreen();
	
	bool BackKeyPressed(const CEGUI::EventArgs& eventName);
	bool LoginKeyPressed(const CEGUI::EventArgs& eventName);
	bool CharacterButtonPressed(const CEGUI::EventArgs& eventName);
};

#endif

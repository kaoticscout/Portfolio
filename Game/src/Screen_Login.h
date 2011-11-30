#ifndef SCREEN_LOGIN_H
#define SCREEN_LOGIN_H

#include "GUIScreen.h"
#include <vector>

class Screen_Login
	: public GUIScreen
{
private:
	CEGUI::Window*					mMainWindow;
	CEGUI::Window*					mWindow;
	CEGUI::Editbox*					mUsernameField;
	CEGUI::Editbox*					mPasswordField;
	CEGUI::PushButton*				mLoginButton;
	CEGUI::PushButton*				mExitButton;
protected:
public:
	Screen_Login(const CEGUI::String& type, const CEGUI::String& name, CEGUI::Window* mainWindow)
		: GUIScreen(type, name)
	{
		mMainWindow = mainWindow;
		Startup();
	}

	virtual ~Screen_Login()
	{
		Shutdown();
	}

	void Startup();
	void Shutdown();
	void Update(DWORD timeSinceLastFrame);
	bool LoginKeyPressed(const CEGUI::EventArgs&);
	bool ExitKeyPressed(const CEGUI::EventArgs&);
};

#endif

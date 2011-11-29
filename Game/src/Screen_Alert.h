#ifndef SCREEN_ALERT_H
#define SCREEN_ALERT_H

#include "GUIScreen.h"
#include "GameAlert.h"
#include <vector>

class Screen_Alert
	: public GUIScreen
{
private:
	CEGUI::Window*			mMainWindow;
	CEGUI::Window*			mWindow;

	//alert log
	CEGUI::Window			*mAlertText;
	CEGUI::String			mCurrentAlert;
	std::vector<GameAlert*> mAlertLog;
	void					LoadAlertLog();
	void					UpdateAlerts();
protected:
public:
	Screen_Alert(const CEGUI::String& type, const CEGUI::String& name, CEGUI::Window* mainWindow)
		: GUIScreen(type, name)
	{
		mMainWindow = mainWindow;
		Startup();
	}

	virtual ~Screen_Alert()
	{
		Shutdown();
	}
	
	void AddAlert(CEGUI::String text);

	void Startup();
	void Shutdown();
	void Update(DWORD timeSinceLastFrame);
};

#endif

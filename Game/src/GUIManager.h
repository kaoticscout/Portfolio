#ifndef GUIMANAGER_H
#define GUIMANAGER_H
#include <CEGUI.h>
#include <OIS.h>
#include <vector>

#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include "CON_GUI.h"
#include "GameComponent.h"
#include "GUIScreen.h"
#include "GameAlert.h"
#include "Screen_Alert.h"

class GUIManager
	: public GameComponent
{
private:
	int mCurrentState;

    CEGUI::OgreRenderer* mRenderer;
	CEGUI::Window *sheet;
	Screen_Alert *mAlertScreen;

	std::vector<GUIScreen*> mActiveSheets;
	
	void InitializeResources();
	void SetUpResources();
	void RestoreDefaultCursor();
	void StateChange();
	void ClearScreens();

public:
	GUIManager();
	~GUIManager();

	void AddAlert(CEGUI::String text);

	void Startup(){};
	void Shutdown(){};
	void Initialize(){};
	bool Update(DWORD timeSinceLastFrame);

	CEGUI::OgreRenderer* GetRenderer() { return mRenderer; }

	//input
	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased( const OIS::KeyEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);
};

#endif



#include "GUIManager.h"
#include "GraphicsEngine.h"
#include "GameEngine.h"
#include "CON_GameConstants.h"
#include "WearableGameObject.h"
#include <string>

#include "Screen_Login.h"
#include "Screen_CharacterSelect.h"
#include "Screen_PlayingLevel.h"
#include "Screen_PauseGame.h"
#include "Screen_Alert.h"
#include "Screen_Logo.h"

void writeFPS()
{
	static int FRAMES = 0;
	static float timeElapsed = 0;
	static float lastCheck = GAMEENGINE.GetTotalTimeElapsed();
	timeElapsed = GAMEENGINE.GetTotalTimeElapsed();
	FRAMES++;
	if(timeElapsed-lastCheck >= 1000.0f)
	{
		lastCheck = timeElapsed;
		std::cout << "FPS:" << FRAMES << std::endl;
		FRAMES = 0;
	}
}

GUIManager::GUIManager()
    : mRenderer(0)
{
	InitializeResources();
	SetUpResources();
}

GUIManager::~GUIManager()
{
	delete sheet;
}

void GUIManager::InitializeResources()
{
    mRenderer = NULL;
	sheet = NULL;
	mCurrentState = -1; //TODO Shouldnt do this. Object should always be in a valid state
}

void GUIManager::SetUpResources()
{
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::Imageset::setDefaultResourceGroup("CEGUI");
    CEGUI::Font::setDefaultResourceGroup("CEGUI");
    CEGUI::Scheme::setDefaultResourceGroup("CEGUI");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("CEGUI");
    CEGUI::WindowManager::setDefaultResourceGroup("CEGUI");

    CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	CEGUI::SchemeManager::getSingleton().create("WindowsLook.scheme");
	CEGUI::SchemeManager::getSingleton().create("VanillaSkin.scheme");
    CEGUI::SchemeManager::getSingleton().create("Items.scheme");

	CEGUI::System::getSingleton().setDefaultFont( "DejaVuSans-10" );

	RestoreDefaultCursor();

    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    sheet = wmgr.createWindow("DefaultWindow");
	CEGUI::System::getSingleton().setGUISheet(sheet);

	mAlertScreen = new Screen_Alert("TaharezLook/StaticImage", "AlertScreen", sheet);
}

bool GUIManager::Update(DWORD timeSinceLastFrame)
{		
	writeFPS();
	if(mCurrentState != GAMEENGINE.GetStateManager()->GetCurrentState())
		StateChange();

	mAlertScreen->Update(timeSinceLastFrame);

	for(size_t counter = 0; counter < mActiveSheets.size(); counter++)
		mActiveSheets[counter]->Update(timeSinceLastFrame);

	CEGUI::System::getSingleton().injectMouseMove(GAMEENGINE.GetMouse()->getMouseState().X.rel*MOUSE_MOVEMENT_SPEED_X, 
												  GAMEENGINE.GetMouse()->getMouseState().Y.rel*MOUSE_MOVEMENT_SPEED_Y);
	  
	return true;
}


void GUIManager::RestoreDefaultCursor()
{
	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
}

void GUIManager::StateChange()
{
	//clear GUI and push new screens, unless currently playing level
	if(mCurrentState == PLAYING_LEVEL)
	{
		//clear gui if going back to login screen
		if(GAMEENGINE.GetStateManager()->GetCurrentState() == LOGGING_OUT)
			ClearScreens();
	}
	else
		ClearScreens();

	mCurrentState = GAMEENGINE.GetStateManager()->GetCurrentState();

	if(GAMEENGINE.GetStateManager()->GetCurrentState() == MAIN_MENU)
	{
		GUIScreen* newScreen = new Screen_Login("TaharezLook/StaticImage", "mainMenuScreen", sheet);
		mActiveSheets.push_back(newScreen);
	}
	else if (GAMEENGINE.GetStateManager()->GetCurrentState() == LOGO)
	{
		GUIScreen* newScreen = new Screen_Logo("TaharezLook/StaticImage", "charSelectionScreen", sheet);
		mActiveSheets.push_back(newScreen);
	}
	else if (GAMEENGINE.GetStateManager()->GetCurrentState() == CHARACTER_SELECTION)
	{
		GUIScreen* newScreen = new Screen_CharacterSelect("TaharezLook/StaticImage", "charSelectionScreen", sheet);
		mActiveSheets.push_back(newScreen);
	}
	else if(GAMEENGINE.GetStateManager()->GetCurrentState() == LOADING_LEVEL)
	{
		//GUIScreen* newScreen = new Screen_PlayingLevel("TaharezLook/StaticImage", "charSelectionScreen", sheet);
		//mActiveSheets.push_back(newScreen);
	}
	else if(GAMEENGINE.GetStateManager()->GetCurrentState() == PLAYING_LEVEL)
	{
		GUIScreen* newScreen = new Screen_PlayingLevel("TaharezLook/StaticImage", "charSelectionScreen", sheet);
		mActiveSheets.push_back(newScreen);
	}
	else if(GAMEENGINE.GetStateManager()->GetCurrentState() == PAUSE_GAME)
	{
		GUIScreen* newScreen = new Screen_PauseGame("TaharezLook/StaticImage", "charSelectionScreen", sheet);
		mActiveSheets.push_back(newScreen);
	}
}


bool GUIManager::keyPressed( const OIS::KeyEvent &arg )
{
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectKeyDown(arg.key);
	sys.injectChar(arg.text);

	return true;
}

bool GUIManager::keyReleased( const OIS::KeyEvent &arg )
{
	return true;
}

bool GUIManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));
	return true;
}

bool GUIManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
	return true;
}

CEGUI::MouseButton GUIManager::convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;
    case OIS::MB_Right:
        return CEGUI::RightButton;
    case OIS::MB_Middle:
        return CEGUI::MiddleButton;
    default:
        return CEGUI::LeftButton;
    }
}

void GUIManager::ClearScreens()
{
	for(std::vector<GUIScreen*>::iterator it = mActiveSheets.begin(); it != mActiveSheets.end(); it++)
		delete *it;
	mActiveSheets.clear();
}

void GUIManager::AddAlert(CEGUI::String text)
{
	mAlertScreen->AddAlert(text);
}


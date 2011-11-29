#include "Screen_PauseGame.h"
#include "GameEngine.h"
#include "CON_GameConstants.h"

void Screen_PauseGame::Startup()
{

	//add background
	mWindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	mWindow->setSize(CEGUI::UVector2(CEGUI::UDim(.4, 0), CEGUI::UDim(.6, 0)));
	mWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(.3, 0), CEGUI::UDim(.2, 0)));
	mWindow->setAlpha(0.8f);

	mMainWindow->addChildWindow(mWindow);

	//add logout button
	mLogoutButton = (CEGUI::PushButton*)CEGUI::WindowManager::getSingleton().createWindow("WindowsLook/Button");
	mWindow->addChildWindow(mLogoutButton);
	mLogoutButton->setArea(CEGUI::URect(CEGUI::UDim(0.2f,0), CEGUI::UDim(0.65f,0), 
								CEGUI::UDim(0.8f,0), CEGUI::UDim(0.75f,0)));
	mLogoutButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&Screen_PauseGame::LogoutKeyPressed, this));
	mLogoutButton->setText("Log out");

	//add Exit Game button
	mLogoutButton = (CEGUI::PushButton*)CEGUI::WindowManager::getSingleton().createWindow("WindowsLook/Button");
	mWindow->addChildWindow(mLogoutButton);
	mLogoutButton->setArea(CEGUI::URect(CEGUI::UDim(0.2f,0), CEGUI::UDim(0.8f,0), 
								CEGUI::UDim(0.8f,0), CEGUI::UDim(0.9f,0)));
	mLogoutButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&Screen_PauseGame::ExitGameKeyPressed, this));
	mLogoutButton->setText("ExitGame");
}

void Screen_PauseGame::Shutdown()
{
	mMainWindow->removeChildWindow(mWindow);
}

void Screen_PauseGame::Update(DWORD timeSinceLastFrame)
{

}

bool Screen_PauseGame::LogoutKeyPressed(const CEGUI::EventArgs& eventName)
{
	GAMEENGINE.GetStateManager()->SetCurrentState(LOGGING_OUT);
	return true;
}

bool Screen_PauseGame::ExitGameKeyPressed(const CEGUI::EventArgs& eventName)
{
	
	GAMEENGINE.GetStateManager()->SetCurrentState(SHUTTING_DOWN);
	return true;
}
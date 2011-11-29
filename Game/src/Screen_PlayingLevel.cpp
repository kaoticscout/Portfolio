#include "Screen_PlayingLevel.h"
#include "GameEngine.h"

void Screen_PlayingLevel::Startup()
{
	//add background
	mWindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	mWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
	mWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	mWindow->setAlpha(0.0);

	mMainWindow->addChildWindow(mWindow);

	//we can edit this frame if wanted
	mHealthBarFrame = CEGUI::WindowManager::getSingleton().createWindow("WindowsLook/StaticImage");
	mMainWindow->addChildWindow(mHealthBarFrame);
	mHealthBarFrame->setSize(CEGUI::UVector2(CEGUI::UDim(0.20f, 0), CEGUI::UDim(0.05f, 0)));
	mHealthBarFrame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05f, 0.0f), CEGUI::UDim(0.05f, 0.0f)));

	mHealthBar = CEGUI::WindowManager::getSingleton().createWindow("WindowsLook/StaticImage");
	mHealthBarFrame->addChildWindow(mHealthBar);
	mHealthBar->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
	mHealthBar->setProperty( "Image", "set:TaharezLook image:HealthBar" );
}

void Screen_PlayingLevel::Update(DWORD timeSinceLastFrame)
{
	//update health bar
	if(GAMEENGINE.GetPlayer() != NULL)
	{
		Ogre::Real pctFill = (Ogre::Real)GAMEENGINE.GetPlayer()->GetCurrentHealth() / (Ogre::Real)GAMEENGINE.GetPlayer()->GetMaxHealth();
		mHealthBar->setSize(CEGUI::UVector2(CEGUI::UDim(pctFill, 0), CEGUI::UDim(1.0f, 0)));
	}
}

void Screen_PlayingLevel::Shutdown()
{
	mMainWindow->removeChildWindow(mWindow);
}
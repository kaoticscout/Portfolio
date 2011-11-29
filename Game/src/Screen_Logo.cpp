#include "Screen_Logo.h"
#include "GameEngine.h"

const float ALPHA_INC = 0.01;
const float TIME_DELAY = 3.0;

void Screen_Logo::Startup()
{	
	mTimer = new boost::timer();
	mDisplaying = false;
	CEGUI::ImagesetManager::getSingletonPtr()->createFromImageFile("logo","logoTrans.png");
	
	//add background
	mWindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
    mWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
	mWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.4, 0)));
	mWindow->setProperty("Image", "set:logo image:full_image" );
	mWindow->setProperty("FrameEnabled", "false");
	mWindow->setAlpha(0);
	
	mMainWindow->addChildWindow(mWindow);
}

void Screen_Logo::Shutdown()
{
	mMainWindow->removeChildWindow(mWindow);
	delete mWindow;
}

void Screen_Logo::Update(DWORD timeSinceLastFrame)
{
	//fade in
	float currentAlpha = mWindow->getAlpha();
	if(!mDisplaying)
	{
		if(currentAlpha < 1) 
		{
			mWindow->setAlpha( currentAlpha + ALPHA_INC );
			mTimer->restart();
		}
		else if(currentAlpha == 1)
		{	
			//fade out after 3 seconds
			if(mTimer->elapsed() >= TIME_DELAY)
			{
				mDisplaying = true;
			}
		}
	}
	else
	{
		if(currentAlpha > 0)
		{
			mWindow->setAlpha( currentAlpha - ALPHA_INC );
			mTimer->restart();
		}
		else 
		{	
			//start game 1 second after faded out
			if(mTimer->elapsed() >= 1.0)
			{
				StartGame();
			}
		}
	}
}

void Screen_Logo::StartGame()
{
	GAMEENGINE.GetStateManager()->SetCurrentState(MAIN_MENU);
}
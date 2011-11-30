#include "Screen_Login.h"
#include "CON_GameConstants.h"
#include "GameEngine.h"
#include "SoundEngine.h"

void Screen_Login::Startup()
{
	SOUNDENGINE.Play_Music("media/music/mainMenuBGMusic.mp3");
	CEGUI::ImagesetManager::getSingletonPtr()->createFromImageFile("mainMenuBG","OgreBG.tga");
	
	//add background
	mWindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	mWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
	mWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	mWindow->setProperty("Image", "set:mainMenuBG image:full_image" );
	mWindow->setProperty("FrameEnabled", "false");
	mWindow->setAlpha(1.0);

	mMainWindow->addChildWindow(mWindow);

	//add login button
	mLoginButton = (CEGUI::PushButton*)CEGUI::WindowManager::getSingleton().createWindow("WindowsLook/Button");
	mWindow->addChildWindow(mLoginButton);
	mLoginButton->setArea(CEGUI::URect(CEGUI::UDim(0.4f,0), CEGUI::UDim(0.56f,0), 
								CEGUI::UDim(0.6f,0), CEGUI::UDim(0.6f,0)));
	mLoginButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&Screen_Login::LoginKeyPressed, this));
	mLoginButton->setText("Login");
	mLoginButton->setFont("DejaVuSans-10");

	//add exit button
	mExitButton = (CEGUI::PushButton*)CEGUI::WindowManager::getSingleton().createWindow("WindowsLook/Button");
	mWindow->addChildWindow(mExitButton);
	mExitButton->setArea(CEGUI::URect(CEGUI::UDim(0.05f,0), CEGUI::UDim(0.9f,0), 
								CEGUI::UDim(0.15f,0), CEGUI::UDim(0.94f,0)));
	mExitButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&Screen_Login::ExitKeyPressed, this));
	mExitButton->setText("Exit");	
	mExitButton->setFont("DejaVuSans-10");

	//add username field
	mUsernameField= (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().createWindow("WindowsLook/Editbox");
	mWindow->addChildWindow(mUsernameField);
	mUsernameField->setArea(CEGUI::URect(CEGUI::UDim(0.40f,0), CEGUI::UDim(0.4f,0), 
								CEGUI::UDim(0.6f,0), CEGUI::UDim(0.44f,0)));
	mUsernameField->setFont("DejaVuSans-10");
	mUsernameField->activate(); 

	//add password field
	mPasswordField = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().createWindow("WindowsLook/Editbox");
	mWindow->addChildWindow(mPasswordField);
	mPasswordField->setArea(CEGUI::URect(CEGUI::UDim(0.4f,0), CEGUI::UDim(0.50f,0), 
								CEGUI::UDim(0.6f,0), CEGUI::UDim(0.54f,0)));
	mPasswordField->setTextMasked(true);
	mPasswordField->setFont("DejaVuSans-10");
}

void Screen_Login::Shutdown()
{
	mMainWindow->removeChildWindow(mWindow);
	delete mWindow;
	delete mUsernameField;
	delete mPasswordField;
	delete mLoginButton;
}

void Screen_Login::Update(DWORD timeSinceLastFrame)
{
	static bool keyDown = false;
	if(GAMEENGINE.GetKeyboard()->isKeyDown(OIS::KC_TAB) && keyDown == false)
	{
		keyDown = true;
		if(mUsernameField->isActive())
			mPasswordField->activate();
		else
			mUsernameField->activate();
	}
	else if(!GAMEENGINE.GetKeyboard()->isKeyDown(OIS::KC_TAB))
		keyDown = false;
}

bool Screen_Login::LoginKeyPressed(const CEGUI::EventArgs& eventName)
{
	SOUNDENGINE.Play_Sound("media/sfx/buttonPress.mp3");
	if((mUsernameField->getText() == "guest" || mUsernameField->getText() == "") && (mPasswordField->getText() == "guest" || mPasswordField->getText() == ""))
		GAMEENGINE.GetStateManager()->SetCurrentState(CHARACTER_SELECTION);
	else
	{
		//GAMEENGINE.GetGUIManager()->AddAlert("Invalid username or password");
		std::cout << "Invalid username or password" << std::endl;
	}
	return true;
}

bool Screen_Login::ExitKeyPressed(const CEGUI::EventArgs& eventName)
{
	SOUNDENGINE.Play_Sound("media/sfx/buttonPress.mp3");
	GAMEENGINE.GetStateManager()->SetCurrentState(SHUTTING_DOWN);
	return true;
}
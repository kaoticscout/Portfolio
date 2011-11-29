#include "Screen_Login.h"
#include "CON_GameConstants.h"
#include "GameEngine.h"

void Screen_Login::Startup()
{
	CEGUI::ImagesetManager::getSingletonPtr()->createFromImageFile("mainMenuBG","background.tga");
	
	//add background
	mWindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	mWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
	mWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	mWindow->setProperty("Image", "set:mainMenuBG image:full_image" );
	mWindow->setAlpha(1.0);

	mMainWindow->addChildWindow(mWindow);

	//add login button
	mLoginButton = (CEGUI::PushButton*)CEGUI::WindowManager::getSingleton().createWindow("WindowsLook/Button");
	mWindow->addChildWindow(mLoginButton);
	mLoginButton->setArea(CEGUI::URect(CEGUI::UDim(0.2f,0), CEGUI::UDim(0.65f,0), 
								CEGUI::UDim(0.3f,0), CEGUI::UDim(0.7f,0)));
	mLoginButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&Screen_Login::LoginKeyPressed, this));
	mLoginButton->setText("Login");

	//add username field
	mUsernameField= (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().createWindow("WindowsLook/Editbox");
	mWindow->addChildWindow(mUsernameField);
	mUsernameField->setArea(CEGUI::URect(CEGUI::UDim(0.11f,0), CEGUI::UDim(0.43f,0), 
								CEGUI::UDim(0.32f,0), CEGUI::UDim(0.48f,0)));
	mUsernameField->activate(); 

	//add password field
	mPasswordField = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().createWindow("WindowsLook/Editbox");
	mWindow->addChildWindow(mPasswordField);
	mPasswordField->setArea(CEGUI::URect(CEGUI::UDim(0.11f,0), CEGUI::UDim(0.54f,0), 
								CEGUI::UDim(0.32f,0), CEGUI::UDim(0.59f,0)));
	mPasswordField->setTextMasked(true);
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

}

bool Screen_Login::LoginKeyPressed(const CEGUI::EventArgs& eventName)
{
	if((mUsernameField->getText() == "guest" || mUsernameField->getText() == "") && (mPasswordField->getText() == "guest" || mPasswordField->getText() == ""))
		GAMEENGINE.GetStateManager()->SetCurrentState(CHARACTER_SELECTION);
	else
	{
		//GAMEENGINE.GetGUIManager()->AddAlert("Invalid username or password");
		std::cout << "Invalid username or password" << std::endl;
	}
	return true;
}
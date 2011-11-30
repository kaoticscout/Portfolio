#include "Screen_CharacterSelect.h"
#include "CON_GameConstants.h"
#include "GameEngine.h"

void Screen_CharacterSelect::Startup()
{
	CEGUI::ImagesetManager::getSingletonPtr()->createFromImageFile("charSelectBG","charSelect.tga");
	CEGUI::ImagesetManager::getSingletonPtr()->createFromImageFile("charSelectIcon","Icon.tga");
	
	//add background
	mWindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	mWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
	mWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	mWindow->setProperty("Image", "set:charSelectBG image:full_image" );
	mWindow->setProperty("FrameEnabled", "false");
	mWindow->setAlpha(1.0);
	mMainWindow->addChildWindow(mWindow);

	loadCharacterScreen();

	//add login button
	mLoginButton = (CEGUI::PushButton*)CEGUI::WindowManager::getSingleton().createWindow("WindowsLook/Button");
	mWindow->addChildWindow(mLoginButton);
	mLoginButton->setArea(CEGUI::URect(CEGUI::UDim(0.85f,0), CEGUI::UDim(0.96f,0), 
								CEGUI::UDim(0.95f,0), CEGUI::UDim(0.995f,0)));
	mLoginButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&Screen_CharacterSelect::LoginKeyPressed, this));
	mLoginButton->setText("Login");

	//add back button
	mBackButton = (CEGUI::PushButton*)CEGUI::WindowManager::getSingleton().createWindow("WindowsLook/Button");
	mWindow->addChildWindow(mBackButton);
	mBackButton->setArea(CEGUI::URect(CEGUI::UDim(0.05f,0), CEGUI::UDim(0.96f,0), 
								CEGUI::UDim(0.15f,0), CEGUI::UDim(0.995f,0)));
	mBackButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&Screen_CharacterSelect::BackKeyPressed, this));
	mBackButton->setText("Back");
}

void Screen_CharacterSelect::Shutdown()
{
		mMainWindow->removeChildWindow(mWindow);
		delete mWindow;
		delete mBackButton;
		delete mLoginButton;
}

void Screen_CharacterSelect::Update(DWORD timeSinceLastFrame)
{
}

bool Screen_CharacterSelect::BackKeyPressed(const CEGUI::EventArgs& eventName) 
{
	GAMEENGINE.GetStateManager()->SetCurrentState(MAIN_MENU);
	return true; 
}
bool Screen_CharacterSelect::LoginKeyPressed(const CEGUI::EventArgs& eventName)
{
	if(mSelectedCharacter != -1)
		GAMEENGINE.GetStateManager()->SetCurrentState(LOADING_LEVEL);
	return true;
}

void Screen_CharacterSelect::loadCharacterScreen()
{
	//add temp char. window
	mCharacterWindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	mCharacterWindow->setSize(CEGUI::UVector2(CEGUI::UDim(.4, 0), CEGUI::UDim(.17, 0)));
	mCharacterWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.23, 0)));
	mCharacterWindow->setAlpha(1.0);
	mWindow->addChildWindow(mCharacterWindow);

	//add temp. char. icon window
	mCharacterIconWindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	mCharacterIconWindow->setSize(CEGUI::UVector2(CEGUI::UDim(.3, 0), CEGUI::UDim(1.0, 0)));
	mCharacterIconWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	mCharacterIconWindow->setProperty("Image", "set:charSelectIcon image:full_image" );
	mCharacterIconWindow->setAlpha(1.0);
	mCharacterWindow->addChildWindow(mCharacterIconWindow);

	//add character text
	mCharacterText = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText");
	mCharacterText->setProperty("FrameEnabled", "false");
	mCharacterText->setProperty("VertFormatting", "VertTop");
	mCharacterText->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.8, 0)));
	mCharacterText->setPosition(CEGUI::UVector2(CEGUI::UDim(.35, 0), CEGUI::UDim(.10f, 0)));
	mCharacterText->setAlpha(1.0);
	mCharacterText->setText("ProcessesChar01\nLevel 01 Warrior\nExperience 0/100");
	mCharacterWindow->addChildWindow(mCharacterText);

	//add button
	mCharacterButton = (CEGUI::PushButton*)CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button");
	mCharacterButton->setAlpha(0.0f);
	mCharacterButton->setSize(CEGUI::UVector2(CEGUI::UDim(.4, 0), CEGUI::UDim(.2, 0)));
	mCharacterButton->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.18, 0)));
	mCharacterButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&Screen_CharacterSelect::CharacterButtonPressed, this));
	mCharacterButton->subscribeEvent(CEGUI::PushButton::EventMouseDoubleClick, CEGUI::Event::Subscriber(&Screen_CharacterSelect::CharacterButtonPressedTwice, this));
	mCharacterButton->setText("");
	mWindow->addChildWindow(mCharacterButton);
}


bool Screen_CharacterSelect::CharacterButtonPressed(const CEGUI::EventArgs& eventName)
{
	mCharacterWindow->setLookNFeel("WindowsLook/StaticImage"); //this is temp, change to a diff taharezLook
	mSelectedCharacter = 1;
	return true;
}

bool Screen_CharacterSelect::CharacterButtonPressedTwice(const CEGUI::EventArgs& eventName)
{
	GAMEENGINE.GetStateManager()->SetCurrentState(LOADING_LEVEL);
	return true;
}
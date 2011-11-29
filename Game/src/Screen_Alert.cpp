#include "Screen_Alert.h"
#include "GameEngine.h"

void Screen_Alert::Startup()
{	
	//add background
	mWindow = CEGUI::WindowManager::getSingleton().createWindow("WindowsLook/StaticText");
	mWindow->setProperty("BackgroundColours", "tl:00000000 tr:00000000 bl:00000000 br:00000000");
	mWindow->setProperty("FrameEnabled", "false");
	mWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
	mWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	mWindow->setAlpha(1.0);
	LoadAlertLog();

	mMainWindow->addChildWindow(mWindow);

}

void Screen_Alert::Shutdown()
{
	mMainWindow->removeChildWindow(mWindow);
	delete mWindow;
}

void Screen_Alert::Update(DWORD timeSinceLastFrame)
{
	UpdateAlerts();
}

void Screen_Alert::AddAlert(CEGUI::String text)
{
	GameAlert* newAlert = new GameAlert(text);
	mAlertLog.push_back(newAlert);
}

void Screen_Alert::LoadAlertLog()
{
	mAlertText = CEGUI::WindowManager::getSingleton().createWindow("WindowsLook/StaticText");
	mMainWindow->addChildWindow(mAlertText);
	mAlertText->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(0.5f, 0)));
	mAlertText->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	mAlertText->setProperty("HorzFormatting", "HorzCentred");
    mAlertText->setProperty("VertFormatting", "VertCentred");
	mAlertText->setProperty("TextColours", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000" );
	mAlertText->setProperty("BackgroundColours", "tl:00000000 tr:00000000 bl:00000000 br:00000000");
	mAlertText->setProperty("FrameEnabled", "false");
	mAlertText->setFont((CEGUI::utf8*)"DejaVuSans-Bold");
}

void Screen_Alert::UpdateAlerts()
{
	mCurrentAlert = "";
	for(std::vector<GameAlert*>::iterator iter = mAlertLog.begin(); iter != mAlertLog.end(); ++iter)
	{
		if(!((*iter)->Update()))
		{
			delete *iter;
			mAlertLog.erase(iter);
			break;
		}
		else	
			mCurrentAlert += (*iter)->GetText() + "\n";
	}

	mAlertText->setText(mCurrentAlert);
}
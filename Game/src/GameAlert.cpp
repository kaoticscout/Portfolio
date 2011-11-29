#include "GameAlert.h"
#include "GameEngine.h"

GameAlert::GameAlert(CEGUI::String text)
{
	mText = text;
	mTimePosted = GAMEENGINE.GetTotalTimeElapsed();
	mActive = true;
}

bool GameAlert::Update()
{
	if((mTimePosted + ALERT_TIME) <= GAMEENGINE.GetTotalTimeElapsed())
		mActive = false;
	return mActive;
}

#ifndef GAMEALERT_H
#define GAMEALERT_H

#include "CEGUIString.h"
#include "CON_GUI.h"

class GameAlert
{
private:
	bool			mActive;
	CEGUI::String	mText;
	float			mTimePosted;
public:
	GameAlert(CEGUI::String text);
	bool Update();
	CEGUI::String GetText() { return mText; }
};
#endif
#ifndef GUISCREEN_H
#define GUISCREEN_H

#include <CEGUI.h>
#include "windows.h"

class GUIScreen
	: public CEGUI::Window
{
public:
	GUIScreen(const CEGUI::String& type, const CEGUI::String& name)
		: CEGUI::Window(type, name)
	{
	}

	virtual ~GUIScreen() {}

	virtual void Startup() {}
	virtual void Shutdown() {}
	virtual void Update(DWORD timeSinceLastFrame) {}
};

#endif
#ifndef GUIELEMENT_H
#define GUIELEMENT_H

#include <CEGUIWindow.h>

class GUIElement
	: public CEGUI::Window
{
public:
	GUIElement(const CEGUI::String& type, const CEGUI::String& name)
		: CEGUI::Window(type, name)
	{
	}

	virtual ~GUIElement() {}

	virtual void Startup() {}
	virtual void Shutdown() {}
	virtual void UpdateWindow() {}
	virtual bool ElementClicked(const CEGUI::EventArgs&) { return true; }
};

#endif
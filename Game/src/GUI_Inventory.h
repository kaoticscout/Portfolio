#ifndef GUI_INVENTORY_H
#define GUI_INVENTORY_H

#include <CEGUIWindow.h>
#include "ItemButton.h"
#include "GUIElement.h"

class GUI_Inventory
	: public GUIElement
{
private:
	CEGUI::Window*		     mWindow;
	std::vector<ItemButton*> mButtonList;

	void Load();
	void LoadInventorySpaces();
	void InitializeInventory();
	void UpdateInventory();
	
public:
	GUI_Inventory(const CEGUI::String& type, const CEGUI::String& name);
	void UpdateWindow();
	bool ElementClicked(const CEGUI::EventArgs&);

};

#endif
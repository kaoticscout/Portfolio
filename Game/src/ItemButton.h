#ifndef ITEM_BUTTON_H
#define ITEM_BUTTON_H

#include <CEGUIWindow.h>
#include "GameObject.h"

class ItemButton
	: public CEGUI::PushButton
{
private:
	GameObject* mItem;
	int itemButtonID;
public:
	ItemButton(const CEGUI::String& type, const CEGUI::String& name)
		: CEGUI::PushButton(type, name), itemButtonID(NULL)
	{
	}
	void SetItem(GameObject* item)
	{
		mItem = item;
	}
	GameObject* GetItem()
	{
		if(mItem != NULL)
			return mItem;
		else
			return NULL;
	}
	int GetItemButtonID() { return itemButtonID; }
	void SetItemButtonID(int value) { itemButtonID = value; }
};

#endif
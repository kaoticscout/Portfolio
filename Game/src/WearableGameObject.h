#ifndef WEARABLEGAMEOBJECT_H
#define WEARABLEGAMEOBJECT_H

#include "OgreString.h"
#include "GameObject.h"

class WearableGameObject
	: public GameObject
{

protected:
	//Object Attributes
	int mStrength;
	int mHealth;
	int mArmor;
	int mStunChance;

public:
	WearableGameObject()
		: GameObject ("Empty", -1, "Default")
	{
		mItemType = 0;
		mItemSlot = -1;
	}
	WearableGameObject(Ogre::String name, int itemSlot, int itemID, Ogre::String textureLocation)
		: GameObject(name, itemID, textureLocation)
	{
		mItemType = 1;
		mItemSlot = itemSlot;
	}

};

#endif
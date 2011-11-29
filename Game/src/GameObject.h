#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "OgreString.h"

class GameObject
{
private:
protected:
	int				mItemType;
	int				mItemID;
	int				mItemSlot;
	int				mQuantity;
	Ogre::String	mName;
	Ogre::String	mTextureLocation;
public:
	GameObject(Ogre::String name, int itemID, Ogre::String textureLocation)
		: mName(name), mItemID(itemID)
	{
		mQuantity = 1;
		mTextureLocation = textureLocation;
		mItemType = 2;
		mItemSlot = -1;
	}
	GameObject(Ogre::String name, int num, int itemID, Ogre::String textureLocation)
		: mName(name), mQuantity(num), mItemID(itemID)
	{
		mTextureLocation = textureLocation;
		mItemType = 2;
		mItemSlot = -1;
	}
	GameObject()
	{
		mItemType = 0;
		mItemID = 0;
		mItemSlot = -1;
		mName = "Default";
		mQuantity=0;
		mTextureLocation="Default";
	}

	//getters
	int GetItemID() { return mItemID; }
	int GetItemSlot() { return mItemSlot; }
	int GetItemType() { return mItemType; }
	int GetQuantity() { return mQuantity; }
	Ogre::String GetName() { return mName; }
	Ogre::String GetItemTexture() { return mTextureLocation; } 

	//setters
	void ModifyQuantity(int value) { mQuantity += value; }
};

#endif
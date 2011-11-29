#include "Inventory.h"
#include "GameObject.h"
#include "GameEngine.h"
#include "WearableGameObject.h"
#include "CON_PlayerConstants.h"

Inventory::Inventory()
{
	InitializeInventory(); //Should just be Initialize();
}

void Inventory::InitializeInventory()
{
	mItems.clear();

	for(int i = 0; i < NUMITEMSLOTS; i++)
	{
		mEquippableItems[i] = WearableGameObject();
	}
	for(int i = 0; i< NUMINVENTORYSLOTS; i++)
	{
		mItems.push_back(GameObject());
	}
}

GameObject* Inventory::GetSpecificItem(int index)
{
	if(index >= 0 && index < mItems.size())
		return &(mItems.at(index)); 
	return NULL;
}

void Inventory::AddItem(GameObject item)
{
	bool itemFound = false;
	int firstAvailableSpot = -1;
	for(int i = 0; i<(int)mItems.size(); i++)
	{
		if( firstAvailableSpot == -1 && mItems[i].GetItemType() == 0 )
			firstAvailableSpot = i;
		if(item.GetItemID() == mItems[i].GetItemID())
		{
			mItems[i].ModifyQuantity(item.GetQuantity());
			itemFound = true;
			break;
		}
	}
	if(!itemFound)
	{
		if(firstAvailableSpot != -1)
			mItems[firstAvailableSpot] = item;
		else
			std::cout << "No inventory space" << std::endl;
	}

	//if(GAMEENGINE.GetGUIManager()->InventoryLoaded())
		//GAMEENGINE.GetGUIManager()->UpdateIcons();
}

void Inventory::RemoveItem(GameObject item)
{
	bool itemFound = false;
	int removeItem = -1;
	for(int i = 0; i<static_cast<int>(mItems.size()); i++)
	{
		if(item.GetItemID() == mItems[i].GetItemID())
		{
			itemFound = true;
			if(item.GetQuantity() > 1)
				item.ModifyQuantity(1);
			else
				removeItem = i;
			break;
		}
	}

	if(removeItem != -1)
		mItems.erase(mItems.begin() + removeItem);
}

void Inventory::UseItem(GameObject* item)
{
	//switch here for what type of item, call various methods such as equip item
	if(item->GetItemType())
	{
	}
}

void Inventory::EquipItem(GameObject* item, int index)
{
	GameObject tempItem = mItems[index-1];
	//if no item in place, just unequip, else swap the current item with the new item
	if(mEquippableItems[item->GetItemSlot()].GetItemSlot() == -1)
	{
		mItems[index-1] = GameObject();
	}
	else
		mItems[index-1] = mEquippableItems[item->GetItemSlot()];
	mEquippableItems[tempItem.GetItemSlot()] = tempItem;
}

void Inventory::EquipItem(GameObject item)
{
	if(mEquippableItems[item.GetItemSlot()].GetItemSlot() != -1)
		AddItem(mEquippableItems[item.GetItemSlot()]);
	mEquippableItems[item.GetItemSlot()] = item;
}

void Inventory::UnequipItem(GameObject* item, int index)
{
	//swap with another item of same type
	if(mItems[index-1].GetItemType() == 1)
	{
		if(mItems[index-1].GetItemSlot() == item->GetItemSlot())
		{
			GameObject tempItem;
			tempItem = mEquippableItems[item->GetItemSlot()];
			mEquippableItems[item->GetItemSlot()] = mItems[index-1];
			mItems[index-1] = tempItem;
		}
		else
		{
			std::cout << "You cannot swap those items" << std::endl;
		}
	}
	//just place in inventory
	else if(mItems[index-1].GetItemType() == 0)
	{
		mItems[index-1] = mEquippableItems[item->GetItemSlot()];
		mEquippableItems[item->GetItemSlot()] = WearableGameObject();
	}
}

void Inventory::PrintInventoryToConsole()
{
	PrintEquipmentToConsole();

	std::cout << "Inventory" << std::endl;
	std::cout << "--------------" << std::endl;
	//make sure vector is not null
	if((int)mItems.size() > 0)
	{
		//display all equipment, display info for empty slot
		for (int i=0; i<(int)mItems.size(); i++)
		{
			if(mItems[i].GetItemID() != 0)
				std::cout << " " << mItems[i].GetName() << " x "<< mItems[i].GetQuantity()<< std::endl;
			else
				std::cout << "No Item" << std::endl;
		}
	}

	std::cout << std::endl;

}

void Inventory::PrintEquipmentToConsole()
{
	std::cout <<"Equipment" << std::endl;
	std::cout << "--------------" << std::endl;

	for(int i = 0; i < NUMITEMSLOTS; i++)
	{
		if(mEquippableItems[i].GetItemID() != 0)
			std::cout << i << ". " << mEquippableItems[i].GetName() << std::endl;
	}

	std::cout << std::endl;
}

void Inventory::SwapItems(GameObject* item1, GameObject* item2)
{
	int firstElement = 0;
	int secondElement = 0;
	
	for(int i = 0; i < mItems.size(); i++)
	{
		if(mItems[i].GetItemID() == item1->GetItemID())
			firstElement = i;
		if(mItems[i].GetItemID() == item2->GetItemID())
			secondElement = i;
	}

	GameObject temp = mItems[firstElement];
	mItems[firstElement] = mItems[secondElement];
	mItems[secondElement] = temp;
}

void Inventory::MoveItem(GameObject* item, int index)
{
	int firstElement = 0;
	for(int i =0; i<mItems.size(); i++)
	{
		if(mItems[i].GetItemID() == item->GetItemID())
			firstElement = i;
	}
	mItems[index-1] = mItems[firstElement];
	mItems[firstElement] = GameObject();
}
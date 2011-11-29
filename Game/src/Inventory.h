#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include "WearableGameObject.h"
#include "CON_PlayerConstants.h"

class Inventory
{
private:
	std::vector<GameObject>		mItems;
	GameObject					mEquippableItems[NUMITEMSLOTS];

	void InitializeInventory();
	void PrintEquipmentToConsole();
protected:
public:
	Inventory();
	
	void AddItem(GameObject item);
	void RemoveItem(GameObject item);
	void EquipItem(GameObject* item, int index);
	void EquipItem(GameObject item);
	void UnequipItem(GameObject* item, int index);
	void UseItem(GameObject* item);
	void SwapItems(GameObject* item1, GameObject* item2);
	void MoveItem(GameObject* item, int index);
	
	int			GetNumItems()  { return mItems.size(); }
	GameObject* GetEquipment() { return mEquippableItems; }

	GameObject* GetSpecificItem(int index);

	//debug
	void PrintInventoryToConsole();
};

#endif


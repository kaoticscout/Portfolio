
#include "GUI_Inventory.h"
#include "GameEngine.h"
#include "Inventory.h"

const int horizInventorySpaces = 6.0f;
void GUI_Inventory::Load()
{
	this->setSize(CEGUI::UVector2(CEGUI::UDim(0.35f, 0), CEGUI::UDim(0.45f, 0)));
	this->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55f, 0.0f), CEGUI::UDim(0.50f, 0.0f)));
	mWindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage",	"Inventory");
	this->addChildWindow(mWindow);
	mWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1.0f,0), CEGUI::UDim(1.0f, 0)));
	//mWindow->setProperty( "Image", "set:Items image:CharacterSheet" );
	LoadInventorySpaces();
	InitializeInventory();
}

void GUI_Inventory::LoadInventorySpaces()
{
	for(int i = 0; i < GAMEENGINE.GetPlayer()->GetInventory()->GetInventorySize(); i++)
	{
		mButtonList.push_back(NULL);
	}

	//Board button grid
	CEGUI::Window* grid = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow");
	mWindow->addChildWindow(grid);
	grid->setXPosition(CEGUI::UDim(HORIZ_PADDING, 0.0f));
	grid->setYPosition(CEGUI::UDim(VERT_PADDING + VERT_OFFSET, 0.0f));

	Ogre::Real numRows = Ogre::Math::Ceil(((Ogre::Real)GAMEENGINE.GetPlayer()->GetInventory()->GetInventorySize() / 6.0f));
	
	float x_inc = 1.0f/ horizInventorySpaces; 
	float y_inc =  1.0f/numRows;

	int itemNum = 0;
	
	CEGUI::Window* invRow;
	for(size_t i = 0 ; i < numRows ; i++)
	{
		//create a container for each row, only leave padding above the top row
		invRow = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow");
		invRow->setArea(CEGUI::URect(CEGUI::UDim(0,0), CEGUI::UDim(x_inc * i, 0), CEGUI::UDim(1,0), CEGUI::UDim(y_inc * (i + 1), 0)));
		grid->addChildWindow(invRow);
		
		for(size_t j = 0 ; j < horizInventorySpaces ; j++)
		{
			if(itemNum < GAMEENGINE.GetPlayer()->GetInventory()->GetInventorySize())
			{
				mButtonList[itemNum] = (ItemButton*)CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button");
				invRow->addChildWindow(mButtonList[itemNum]);
				mButtonList[itemNum]->setArea(CEGUI::URect(CEGUI::UDim(x_inc * j, 0), CEGUI::UDim(0,0),
											CEGUI::UDim(y_inc * (j + 1), 0), CEGUI::UDim(1,0)));
				mButtonList[itemNum]->subscribeEvent(CEGUI::PushButton::EventActivated, CEGUI::Event::Subscriber(&GUI_Inventory::ElementClicked, this));
				mButtonList[itemNum]->setEnabled(true);
				mButtonList[itemNum]->SetItemButtonType((int)BUTTONINV);
				mButtonList[itemNum]->SetItemButtonIndex(itemNum);
				itemNum++;
			}
		}	
	}
	
	grid->setHeight(CEGUI::UDim(grid->getHeight().d_scale - VERT_OFFSET - (2 * VERT_PADDING), 0));
	grid->setWidth(CEGUI::UDim(grid->getWidth().d_scale - (2 * HORIZ_PADDING), 0));
}

void GUI_Inventory::InitializeInventory()
{
	if(GAMEENGINE.GetPlayer() != NULL)
	{
		GameObject* item;
		Inventory* inven = GAMEENGINE.GetPlayer()->GetInventory();
	
		for(int itemNum = 0; itemNum < inven->GetInventorySize(); itemNum++)
		{
			item = (inven->GetSpecificItem(itemNum));
			mButtonList[itemNum]->SetItem(item);
		}
	}
}

void GUI_Inventory::UpdateInventory()
{
	if(GAMEENGINE.GetPlayer() != NULL)
	{
		Inventory* inven = GAMEENGINE.GetPlayer()->GetInventory();
		int itemNum = 0;
		GameObject* item;

		for(int itemNum = 0; itemNum < inven->GetInventorySize(); itemNum++)
		{
			item = (inven->GetSpecificItem(itemNum));
			mButtonList[itemNum]->SetItem(item);
		}

		for(int i = 0; i < 6; i++)
		{
			for(int j = 0; j < 6; j++)
			{
				if(itemNum < GAMEENGINE.GetPlayer()->GetInventory()->GetInventorySize())
				{
					item = (mButtonList[itemNum])->GetItem();
					if(item != NULL)
					{
						Ogre::String itemName = item->GetItemTexture();
						Ogre::String prefixText = "set:Items image:";

						mButtonList[itemNum]->setProperty("NormalImage", prefixText + itemName);
						mButtonList[itemNum]->setProperty("HoverImage", prefixText + itemName);
					}
				}
				itemNum++;
			}
		}
	}
}

GUI_Inventory::GUI_Inventory(const CEGUI::String& type, const CEGUI::String& name)
		: GUIElement(type, name)
{
	Load();
}

void GUI_Inventory::UpdateWindow()
{
	UpdateInventory();
}

bool GUI_Inventory::ElementClicked(const CEGUI::EventArgs& event) 
{
	const CEGUI::WindowEventArgs& win = static_cast<const CEGUI::WindowEventArgs&>(event);
	ItemButton* currentButton = static_cast<ItemButton*>(win.window);
	win.window->deactivate();

	//GAMEENGINE.GetGUIManager()->ItemClicked(currentButton);

	return true;
}


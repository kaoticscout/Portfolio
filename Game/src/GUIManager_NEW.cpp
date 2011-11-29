#include "GUIManager.h"
#include "GraphicsEngine.h"
#include "GameEngine.h"
#include "CON_GameConstants.h"
#include "WearableGameObject.h"


#include <string>

GUIManager::GUIManager()
    : mRenderer(0), itemSelected(NULL)
{
	InitializeResources();
	SetUpResources();
	RegisteredFrameListener::Startup();
}

GUIManager::~GUIManager()
{
	active = false;
}

void GUIManager::InitializeResources()
{
	itemSelected = NULL;
    mRenderer = NULL;
	sheet = NULL;
	inventoryWindow = NULL;
	characterSheet = NULL;
	mCurrentState = -1;
}

void GUIManager::SetUpResources()
{
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::Imageset::setDefaultResourceGroup("CEGUI");
    CEGUI::Font::setDefaultResourceGroup("CEGUI");
    CEGUI::Scheme::setDefaultResourceGroup("CEGUI");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("CEGUI");
    CEGUI::WindowManager::setDefaultResourceGroup("CEGUI");
 
    CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
    CEGUI::SchemeManager::getSingleton().create("Items.scheme");
	CEGUI::SchemeManager::getSingleton().create("WindowsLook.scheme");

	CEGUI::System::getSingleton().setDefaultFont( "DejaVuSans-10" );

	RestoreDefaultCursor();

    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");

	CEGUI::System::getSingleton().setGUISheet(sheet);

	active = true;
 
}

void GUIManager::ResizeWindows()
{	
	sheet->setHeight(CEGUI::UDim(GRAPHICSENGINE.GetViewport()->getActualHeight(), 0));
	sheet->setWidth(CEGUI::UDim(GRAPHICSENGINE.GetViewport()->getActualWidth(), 0));
}

void GUIManager::LoadInventory()
{
	inventoryWindow = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage", "Inventory");
	inventoryWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.35f, 0), CEGUI::UDim(0.45f, 0)));
	inventoryWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55f, 0.0f), CEGUI::UDim(0.50f, 0.0f)));
	inventoryWindow->setAlpha(0.8);
}

void GUIManager::LoadInventorySpaces()
{
	//Board button grid
	grid = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow");
    inventoryWindow->addChildWindow(grid);
    grid->setXPosition(CEGUI::UDim(HORIZ_PADDING, 0.0f));
    grid->setYPosition(CEGUI::UDim(VERT_PADDING + VERT_OFFSET, 0.0f));

    float d_inc = 1.0f/ inventorySize; 
	int itemNum = 1;
	
	invRow;
    for(size_t i = 0 ; i < inventorySize ; i++)
    {
        //create a container for each row, only leave padding above the top row
		invRow = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow");
		invRow->setArea(CEGUI::URect(CEGUI::UDim(0,0), CEGUI::UDim(d_inc * i, 0), CEGUI::UDim(1,0), CEGUI::UDim(d_inc * (i + 1), 0)));
        grid->addChildWindow(invRow);
		
        for(size_t j = 0 ; j < inventorySize ; j++)
        {
            d_buttons[i][j] = (ItemButton*)CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button");
            invRow->addChildWindow(d_buttons[i][j]);
            d_buttons[i][j]->setArea(CEGUI::URect(CEGUI::UDim(d_inc * j, 0), CEGUI::UDim(0,0),
									 CEGUI::UDim(d_inc * (j + 1), 0), CEGUI::UDim(1,0)));
			d_buttons[i][j]->subscribeEvent(CEGUI::PushButton::EventActivated, CEGUI::Event::Subscriber(&GUIManager::ItemClicked, this));
            d_buttons[i][j]->setEnabled(true);
			d_buttons[i][j]->setID(itemNum);
			//d_buttons[i][j]->SetItemButtonID(itemNum);
			itemNum++;
        }	
    }
	
	grid->setHeight(CEGUI::UDim(grid->getHeight().d_scale - VERT_OFFSET - (2 * VERT_PADDING), 0));
	grid->setWidth(CEGUI::UDim(grid->getWidth().d_scale - (2 * HORIZ_PADDING), 0));
}

void GUIManager::UpdateIcons()
{
	int itemCounter = 0;
	for(int i = 0; i < inventorySize; i++)
	{
		for(int j = 0; j < inventorySize; j++)
		{
			if(itemCounter < GAMEENGINE.GetPlayer()->GetInventory()->GetNumItems())
			{
				Ogre::String itemName = d_buttons[i][j]->GetItem()->GetItemTexture();
				Ogre::String prefixText = "set:Items image:";

				d_buttons[i][j]->setProperty("NormalImage", prefixText + itemName);
				d_buttons[i][j]->setProperty("HoverImage", prefixText + itemName);
			}
			itemCounter++;
		}
	}
}

void GUIManager::InitializeInventory()
{
	int itemNum = 0;
	GameObject* item;
	Inventory* inven = GAMEENGINE.GetPlayer()->GetInventory();

	for(size_t i = 0 ; i < inventorySize ; i++)
	{
		for(size_t j = 0 ; j < inventorySize ; j++)
		{
			item = inven->GetSpecificItem(itemNum);
			d_buttons[i][j]->SetItem(item);
			itemNum++;
		}
	}

	UpdateIcons();
}

void GUIManager::LoadCharacterSheet()
{
	characterSheet = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage", "CharacterSheet");
	characterSheet->setSize(CEGUI::UVector2(CEGUI::UDim(0.30f, 0), CEGUI::UDim(0.45f, 0)));
	characterSheet->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1f, 0.0f), CEGUI::UDim(0.4f, 0.0f)));
	characterSheet->setProperty( "Image", "set:Items image:CharacterSheet" );
	characterSheet->setAlpha(1.0);
}

void GUIManager::LoadCharacterSheetSlots()
{
	
	int buttonID = -1;
	// Board button grid
	charGrid = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow");
    characterSheet->addChildWindow(charGrid);
    charGrid->setXPosition(CEGUI::UDim(0.0f, 0.0f));
    charGrid->setYPosition(CEGUI::UDim(0.0f, 0.0f));
	charGrid->setHeight(CEGUI::UDim(1, 0));
	charGrid->setWidth(CEGUI::UDim(1, 0));

	
	for(int i = 0; i < NUMITEMSLOTS; i++)
	{
		char_buttons[i] = (ItemButton*)CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Button");
		charGrid->addChildWindow(char_buttons[i]);

		char_buttons[i]->subscribeEvent(CEGUI::PushButton::EventActivated, CEGUI::Event::Subscriber(&GUIManager::ItemClicked, this));
		char_buttons[i]->SetItemButtonID(buttonID);
		char_buttons[i]->setEnabled(true);
		buttonID--;
	}
	
	
	//manually place buttons
	//headpiece
    char_buttons[0]->setArea(CEGUI::URect(CEGUI::UDim(0.39f, 0), CEGUI::UDim(0.15f,0),
										  CEGUI::UDim(0.57f, 0), CEGUI::UDim(0.34f,0)));
	//chest
    char_buttons[1]->setArea(CEGUI::URect(CEGUI::UDim(0.39f, 0), CEGUI::UDim(0.37f,0),
										  CEGUI::UDim(0.57f, 0), CEGUI::UDim(0.56f,0)));
	//gloves
	char_buttons[2]->setArea(CEGUI::URect(CEGUI::UDim(0.113f, 0), CEGUI::UDim(0.657f,0),
										  CEGUI::UDim(0.293f, 0), CEGUI::UDim(0.847f,0)));
	//boots
	char_buttons[3]->setArea(CEGUI::URect(CEGUI::UDim(0.672f, 0), CEGUI::UDim(0.657f,0),
										  CEGUI::UDim(0.852f, 0), CEGUI::UDim(0.847f,0)));
	//MH_WEAPON
	char_buttons[4]->setArea(CEGUI::URect(CEGUI::UDim(0.114f, 0), CEGUI::UDim(0.38f,0),
										  CEGUI::UDim(0.294f, 0), CEGUI::UDim(0.57f,0)));
	//OH_WEAPON
	char_buttons[5]->setArea(CEGUI::URect(CEGUI::UDim(0.67f, 0), CEGUI::UDim(0.38f,0),
										  CEGUI::UDim(0.85f, 0), CEGUI::UDim(0.57f,0)));
	

}

void GUIManager::InitializeCharacterSheet()
{
	int itemNum = 0;
	Inventory* inven = GAMEENGINE.GetPlayer()->GetInventory();
	GameObject* item;

	for(size_t i = 0 ; i < NUMITEMSLOTS ; ++i)
	{
		item = &(inven->GetEquipment()[i]);
		if(item->GetItemID() == -1)
			char_buttons[i]->SetItem(NULL);
		else
			char_buttons[i]->SetItem(item);
		itemNum++;
	}

	UpdateCharacterSheetIcons();
}

void GUIManager::UpdateCharacterSheetIcons()
{
	for(int i = 0; i < NUMITEMSLOTS; i++)
	{
		if(char_buttons[i]->GetItem() != NULL && char_buttons[i]->GetItem()->GetItemType() != -1 && char_buttons[i]->GetItem()->GetItemTexture() != "Default")
		{
			Ogre::String itemName = char_buttons[i]->GetItem()->GetItemTexture();
			Ogre::String prefixText = "set:Items image:";

			char_buttons[i]->setProperty("NormalImage", prefixText + itemName);
			char_buttons[i]->setProperty("HoverImage", prefixText + itemName);
		}
		else if(char_buttons[i]->GetItem() != NULL && char_buttons[i]->GetItem()->GetItemType() != -1)
		{
			char_buttons[i]->setProperty("NormalImage", "NULL");
			char_buttons[i]->setProperty("HoverImage", "NULL");
		}
	}
}

void GUIManager::ToggleInventoryWindow()
{
	static bool inventoryDisplayed = false;
	if(!inventoryDisplayed)
	{
		sheet->addChildWindow(inventoryWindow);
		inventoryDisplayed = true;
	}
	else 
	{
		inventoryDisplayed = false;
		delete itemSelected;
		itemSelected = NULL;
		sheet->removeChildWindow(inventoryWindow);
	}
}

void GUIManager::ToggleCharacterSheet()
{
	static bool charSheetDisplayed = false;
	if(!charSheetDisplayed)
	{
		sheet->addChildWindow(characterSheet);
		charSheetDisplayed = true;
	}
	else 
	{
		charSheetDisplayed = false;
		sheet->removeChildWindow(characterSheet);
	}
}

bool GUIManager::Update(const FrameEvent& evt)
{		
	static bool clickedMouse = false;

	if(mCurrentState != GAMEENGINE.GetStateManager()->GetCurrentState())
		StateChange();

	CEGUI::System::getSingleton().injectMouseMove(GAMEENGINE.GetMouse()->getMouseState().X.rel, GAMEENGINE.GetMouse()->getMouseState().Y.rel);
	
	if(!clickedMouse && GAMEENGINE.GetMouse()->getMouseState().buttonDown(OIS::MB_Left))
	{
		CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
		CEGUI::System::getSingleton().injectMouseButtonClick(CEGUI::LeftButton);
		clickedMouse = true;
	}
	else if(!GAMEENGINE.GetMouse()->getMouseState().buttonDown(OIS::MB_Left))
		clickedMouse = false;

	return true;
}

bool GUIManager::HandleGameExitClicked(const CEGUI::EventArgs&)
{
	GRAPHICSENGINE.StopRenderLoop();
	return true;
}

bool GUIManager::ItemClicked(const CEGUI::EventArgs& event)
{
	const CEGUI::WindowEventArgs& win = static_cast<const CEGUI::WindowEventArgs&>(event);
	ItemButton* currentButton = static_cast<ItemButton*>(win.window);
	win.window->deactivate();

	//allow the user to select an item if no item is currently selected
	//do some error checkin to make sure user does not select item's not in inventory
	//when an item is selected, let them swap with another item
	if(GAMEENGINE.GetMouse()->getMouseState().buttonDown(OIS::MB_Left))
	{
		if(itemSelected == NULL)
		{
			if(currentButton->GetItem() != NULL && currentButton->GetItem()->GetItemType() != 0)
			{
				itemSelected = currentButton->GetItem();
				if(currentButton->getID() == 0)
					itemSelectedID = currentButton->GetItemButtonID();
				else
					itemSelectedID = currentButton->getID();
				std::cout << "Selected " << itemSelected->GetName() << std::endl;
				CEGUI::System::getSingleton().setDefaultMouseCursor("Items", currentButton->GetItem()->GetItemTexture());
			}
		}
		else
		{
			if(itemSelected != currentButton->GetItem())
			{
				//swap items
				if(itemSelectedID >= 0 && currentButton->getID() > 0 && currentButton->GetItem()->GetItemType() != NULL)
				{
					GAMEENGINE.GetPlayer()->GetInventory()->SwapItems(itemSelected, currentButton->GetItem());
				}
				//equip item
				else if(itemSelectedID > 0 && currentButton->GetItemButtonID() < 0)
				{
					if(itemSelected->GetItemType() == 1)
					{
						int currentItem = (currentButton->GetItemButtonID() * -1) - 1;
						if(itemSelected->GetItemSlot() == currentItem)
						{
							std::cout << "Swap items" << std::endl;
							GAMEENGINE.GetPlayer()->GetInventory()->EquipItem(itemSelected, itemSelectedID);
							char_buttons[currentItem]->SetItem(&(GAMEENGINE.GetPlayer()->GetInventory()->GetEquipment()[currentItem]));
						}
						else
						{
							std::cout << "This item does not belong here." << std::endl;
						}
					}
					else
					{
						std::cout << "You cannot equip this item." << std::endl;
					}
				}

				//unequip item
				else if(itemSelectedID < 0 && currentButton->getID() > 0)
				{
					GAMEENGINE.GetPlayer()->GetInventory()->UnequipItem(itemSelected, currentButton->getID());
				}

				//try to move equipment item to another equipment slot
				else if(itemSelectedID <0 && currentButton->GetItemButtonID() < 0)
				{

				}

				//move item
				else if(itemSelectedID >= 0 && currentButton->getID() > 0)
				{
					GAMEENGINE.GetPlayer()->GetInventory()->MoveItem(itemSelected, currentButton->getID());
				}
				
				UpdateCharacterSheetIcons();
				UpdateIcons();
			}
			RestoreDefaultCursor();
			itemSelected = NULL;
		}
	}
	return false;
}

void GUIManager::RestoreDefaultCursor()
{
	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
}

void GUIManager::StateChange()
{
	if(mCurrentState==MAIN_MENU)
		sheet->removeChildWindow("mainMenu");

	mCurrentState = GAMEENGINE.GetStateManager()->GetCurrentState();

	if(GAMEENGINE.GetStateManager()->GetCurrentState() == MAIN_MENU)
		LoadMainMenu();
	if(GAMEENGINE.GetStateManager()->GetCurrentState() == PLAYING_LEVEL)
	{
		LoadInventory();
		LoadInventorySpaces(); 
		InitializeInventory();

		LoadCharacterSheet();
		LoadCharacterSheetSlots();
		InitializeCharacterSheet();
	}
}

void GUIManager::LoadMainMenu()
{
	CEGUI::ImagesetManager::getSingletonPtr()->createFromImageFile("mainMenuBG","background.tga");
	
	//add background
	CEGUI::Window *mainMenu = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage", "mainMenu");
	sheet->addChildWindow(mainMenu);
	mainMenu->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
	mainMenu->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	mainMenu->setProperty("Image", "set:mainMenuBG image:full_image" );
	mainMenu->setAlpha(1.0);

	CEGUI::PushButton* login = (CEGUI::PushButton*)CEGUI::WindowManager::getSingleton().createWindow("WindowsLook/Button");
	mainMenu->addChildWindow(login);
	login->setArea(CEGUI::URect(CEGUI::UDim(0.2f,0), CEGUI::UDim(0.65f,0), 
							  CEGUI::UDim(0.3f,0), CEGUI::UDim(0.7f,0)));
	login->subscribeEvent(CEGUI::PushButton::EventActivated, CEGUI::Event::Subscriber(&GUIManager::HandleGameLoginClicked, this));
}

bool GUIManager::HandleGameLoginClicked(const CEGUI::EventArgs&)
{
	GAMEENGINE.GetStateManager()->SetCurrentState(PLAYING_LEVEL);
	return true;
}
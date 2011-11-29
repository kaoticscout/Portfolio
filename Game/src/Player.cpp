#include "Player.h"
#include "GameEngine.h"
#include "GraphicsEngine.h"
#include "OIS.h"
#include "CON_PlayerConstants.h"

Player::Player()
	: Character(CT_PLAYER)
{
	Initialize();
	SetName("Player");
	mCharacterStateManager->SetCurrentState(PLAYER_IDLE);
}

Player::~Player()
{
	Shutdown();
}

void Player::Shutdown()
{
	mCharacterNode->detachAllObjects();
	
	GRAPHICSENGINE.GetSceneManager()->destroyLight("pointLight");
	delete mCharacterNode;
	delete mCharacterEntity;
	delete mAnimationHelper;

	mCharacterNode = NULL;
	mCharacterEntity = NULL;
}

void Player::Initialize()
{
	SetUpBody();
	SetupWeapons();
	SetUpCharacterStats();
	SetUpInventory();
	SetUpLightRadius();
	
	mAnimationHelper = new AnimationHelper(mCharacterEntity);
	SetUpAnimations();
}

void Player::SetUpCharacterStats()
{
	mIsActive = true;
	mMaxHealth = 100;
	mCurrentHealth = mMaxHealth;
	mAgility = 1;
	mStrength = 1;
	mArmor = 0;
	mMovementScale = 1;
	mSightRadius = 0;
	mStunChance = 0;
	mAttackCD = 500;
	mCurrentAttackCD = 0;
}

void Player::SetUpBody()
{
	mKeyDirection = Ogre::Vector3(0,0,0);
	mMaxSpeed = PLAYER_BASE_MOVE_SPEED;
	mRotation = 0;

	//Create body
	mCharacterEntity = GRAPHICSENGINE.GetSceneManager()->createEntity(PLAYERMODELLOC);
	mCharacterNode = GRAPHICSENGINE.GetSceneManager()->getRootSceneNode()->createChildSceneNode(Vector3::UNIT_Y * CHAR_HEIGHT);
	mCharacterNode->pitch(Ogre::Degree(-90));
	mCharacterEntity->setCastShadows(true);

	tempNode = GRAPHICSENGINE.GetSceneManager()->createSceneNode();
	tempNode->attachObject(mCharacterEntity);
	mCharacterNode->addChild(tempNode);

	//Attach camera
	Ogre::Camera* cam = GRAPHICSENGINE.GetSceneManager()->getCamera("MainCamera");
	mCharacterNode->attachObject(cam);
	
	SetCharacterOrientationToHeading();
}

void Player::SetupWeapons()
{
	//Create weapons and attach to sheath
	mMainHandWeapon = GRAPHICSENGINE.GetSceneManager()->createEntity("Axe.mesh");
	mOffHandWeapon = GRAPHICSENGINE.GetSceneManager()->createEntity( "Sword02.mesh");
	mCharacterEntity->attachObjectToBone("Sheath.L", mMainHandWeapon);
	mCharacterEntity->attachObjectToBone("Sheath.R", mOffHandWeapon);
}

void Player::SetUpInventory()
{
	mInventory = Inventory();

	//temporary startup items
	mInventory.EquipItem(WearableGameObject("Helm of Greatness", (int)SLOT_HEAD, 1, "Helm01"));
	mInventory.EquipItem(WearableGameObject("Chestplate of Uber", (int)SLOT_CHEST, 2, "Chest01"));
	mInventory.EquipItem(WearableGameObject("Sword of Awesome", (int)SLOT_MH_WEAPON, 3, "Sword01"));
	mInventory.EquipItem(WearableGameObject("Sword of almost as Awesome", (int)SLOT_OH_WEAPON, 4, "Sword02"));
	
	mInventory.AddItem(WearableGameObject("Mediocre Shirt", (int)SLOT_CHEST, 5, "Shirt01"));
	mInventory.AddItem(WearableGameObject("Noobish Sword of the Downs", (int)SLOT_OH_WEAPON, 6, "Sword03"));
	mInventory.AddItem(GameObject("Healing Potion", 3, 7, "HealingPotion01"));
	mInventory.AddItem(WearableGameObject("Boots of Speediness", (int)SLOT_BOOTS, 8, "Boots01"));
}

void Player::SetUpLightRadius()
{
	mAvatarPointLight = GRAPHICSENGINE.GetSceneManager()->createLight("pointLight");
    
	mAvatarPointLight->setType(Ogre::Light::LT_POINT);
    mAvatarPointLight->setPosition(Ogre::Vector3(0, 0, 0));
    mAvatarPointLight->setDiffuseColour(0.7,0.7, 0.5);
    mAvatarPointLight->setSpecularColour(0.7,0.7, 0.5);

	mCharacterNode->attachObject(mAvatarPointLight);
}

void Player::SetUpAnimations()
{
	mCharacterEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

	mAnimationHelper->SetBottomAnimation("IdleBase");
	mAnimationHelper->SetTopAnimation("IdleTop", false);

	mWeaponDrawn = false;
}

bool Player::Update(DWORD timeSinceLastFrame)
{
	if(GAMEENGINE.GetStateManager()->GetCurrentState() != PAUSE_GAME)
		AnimateBasedOnMovement();
	
	mCurrentAttackCD -= timeSinceLastFrame;
	mAnimationHelper->UpdateAnimations(timeSinceLastFrame);
	CheckInput();	
	SetCharacterOrientationToHeading();
	return true; 
}

void Player::Collision(CollisionObject* other)
{
		
}

void Player::CheckInput()
{
	static OIS::Keyboard* key = GAMEENGINE.GetKeyboard();
	static bool WeaponKey = false;
	
	if(mIsActive)
	{
		bool ForwardKey = key->isKeyDown(OIS::KC_W)||key->isKeyDown(OIS::KC_UP) ? true:false;
		bool RightKey = key->isKeyDown(OIS::KC_D)||key->isKeyDown(OIS::KC_RIGHT) ? true:false;
		bool LeftKey = key->isKeyDown(OIS::KC_A)||key->isKeyDown(OIS::KC_LEFT) ? true:false;
		bool BackKey = key->isKeyDown(OIS::KC_S)||key->isKeyDown(OIS::KC_DOWN) ? true:false;
	
		//Player Movement
		mKeyDirection.z = ForwardKey && !BackKey ? -1 : 0;
		if (BackKey && !ForwardKey)
			mKeyDirection.z = .2;

		if(RightKey)
			mRotation += PLAYER_TURN_SPEED;
		if(LeftKey)
			mRotation -= PLAYER_TURN_SPEED;

		if (mKeyDirection.z < 0)
		{
			MoveCharacterForward();
			SetVelocity(GetHeading()*mMaxSpeed);
		}
		else if (mKeyDirection.z > 0)
		{
			MoveCharacterBackward();
			SetVelocity(GetHeading()*-mMaxSpeed);
		} 
		else 
		{
			SetVelocity(Ogre::Vector3::ZERO);
		}

		//Draw weapons
		if (key->isKeyDown(OIS::KC_Q))
		{
			if(!WeaponKey)
			{
				WeaponKey = true;
				mAnimationHelper->SetTopAnimation("DrawSwords", true);			
			
				if(!mWeaponDrawn)
				{
					mCharacterEntity->detachAllObjectsFromBone();
					mCharacterEntity->attachObjectToBone(mWeaponDrawn ? "Sheath.L" : "Handle.L", mMainHandWeapon);
					mCharacterEntity->attachObjectToBone(mWeaponDrawn ? "Sheath.R" : "Handle.R", mOffHandWeapon);
				}
				else
				{	
					mCharacterEntity->detachAllObjectsFromBone();
					mCharacterEntity->attachObjectToBone("Sheath.L", mMainHandWeapon);
					mCharacterEntity->attachObjectToBone("Sheath.R", mOffHandWeapon);
				}
				mWeaponDrawn = !mWeaponDrawn;
			}
		}
		else 
			WeaponKey = false;

		//Attack type 1
		if (key->isKeyDown(OIS::KC_1) && mWeaponDrawn && mCurrentAttackCD <= 0)
		{
			std:: cout << "Attack1" << std::endl;
			mAnimationHelper->SetTopAnimation("SliceVertical", true);
			mCurrentAttackCD = mAttackCD;

			std::vector<Character*> obj = GAMEENGINE.GetCollisionManager()->CheckCollisions(this, 1);
			if(!obj.empty())
			{
				Attack(obj, 5, 1);
			}
		}
		//Attack type 2
		else if(key->isKeyDown(OIS::KC_2) && mWeaponDrawn && mCurrentAttackCD <= 0)
		{	
			std:: cout << "Attack2" << std::endl;
			mAnimationHelper->SetTopAnimation("SliceHorizontal", true);
			mCurrentAttackCD = mAttackCD;

			std::vector<Character*> obj = GAMEENGINE.GetCollisionManager()->CheckCollisions(this, 2);
			if(!obj.empty())
			{
				Attack(obj, 3, 2);
			}
		}

		AnimateBasedOnMovement();

		//TEMP way to adjust player health to test certain senarios
		if(key->isKeyDown(OIS::KC_M))
		{
			if(mCurrentHealth<mMaxHealth)
				mCurrentHealth++;
		}
		else if(key->isKeyDown(OIS::KC_N))
		{	
			if(mCurrentHealth > 0)
				mCurrentHealth--;
		}
	}
}

void Player::AnimateBasedOnMovement()
{
	if (mVelocity.x != 0 || mVelocity.y != 0 || mVelocity.z != 0)
	{
		mAnimationHelper->SetBottomAnimation("RunBase");
		mAnimationHelper->SetTopAnimation("RunTop", false);
	}
	else 
	{
		mAnimationHelper->SetBottomAnimation("IdleBase");
		mAnimationHelper->SetTopAnimation("IdleTop", false);
	}
}

bool Player::CheckDeath()
{
	if(mCurrentHealth <= 0)
	{	
		GAMEENGINE.GetGUIManager()->AddAlert("Game Over!");
		GAMEENGINE.GetGUIManager()->AddAlert("Thanks for playing.");

		CollisionObject::Shutdown();
		tempNode->pitch(Ogre::Degree(-90));
		mCharacterNode->setPosition(-100000,-100000,-100000); //temporarily port unit off screen
		SetActive(false);
		return true;
	}
	else
		return false;
}
#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "Inventory.h"

class Player :
	public Character
{

private:
	Ogre::SceneNode* tempNode;
	Inventory		mInventory;  //Player inventory manager
	Ogre::Light*	mAvatarPointLight; //Light that illuminates the players avatar

	//Constraint variables
	int			    mExperienceNeededToLevel;
	int				mMaxAvailableGameLevel;

	//Player attribute variables
	int				mAvailableAttributePoints;
	int				mCurrentExperience;
	Ogre::Real      mSightRadius;
	Ogre::Real		mStunChance;
	
	//Weapon variables
	bool			mWeaponDrawn;
	Ogre::Entity*	mMainHandWeapon;
	Ogre::Entity*	mOffHandWeapon;

	//Movement variables
	Vector3			mGoalDirection;					// actual intended direction in world-space
	Vector3			mKeyDirection;					// player's local intended direction based on input keys
	Ogre::Real		mMovementScale;

	Ogre::Real		mAttackCD;
	Ogre::Real		mCurrentAttackCD;
public:
	Player();
	~Player();

	void Initialize();
	void Startup(){}
	void Shutdown();
	bool Update(DWORD timeSinceLastFrame);  //For Player game logic

	void Collision(CollisionObject* other);

	//Player Actions
	void AcquireItem();
	void DestroyItem();

	//Player Maintenance
	void AddExp(int value);
	bool CheckDeath();
	void CheckInput();
	bool CheckLevel();
	void HandleMovement();
	void IncrementLevel();

	//Player Initialization
	void SetUpBody();
	void SetUpCharacterStats();
	void SetUpInventory();
	void SetUpLightRadius();
	void SetupWeapons();

	//Animation methods	
	void SetUpAnimations();
	void AnimateBasedOnMovement();

	//Getters
	int				 GetAvailableAttributePoints() const	{ return mAvailableAttributePoints; }
	int				 GetCurrentExperience()		   const	{ return mCurrentExperience; }
	int				 GetExperienceNeededToLevel()  const	{ return mExperienceNeededToLevel; }
	Inventory*		 GetInventory()							{ return &mInventory; }
	int				 GetMaxAvailableLevel()		   const	{ return mMaxAvailableGameLevel; }
	Ogre::Real		 GetMovementScale()			   const	{ return mMovementScale; }
	Ogre::SceneNode* GetSceneNode()				   const	{ return mCharacterNode; }
	Ogre::Real		 GetSightRadius()			   const	{ return mSightRadius; }
	Ogre::Real		 GetStunChance()			   const	{ return mStunChance; }
};

#endif


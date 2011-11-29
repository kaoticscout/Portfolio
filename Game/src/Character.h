#ifndef CHARACTER_H
#define CHARACTER_H

#include "CollisionObject.h"
#include "OgreString.h"
#include "StateManager.h"
#include "AnimationHelper.h"

class Character
	: public CollisionObject
{
protected:
	StateManager*		mCharacterStateManager;
	
	bool				mIsActive;
	int					mCurrentHealth;
	int					mMaxHealth;
	int					mAgility;
	int					mStrength;
	int					mArmor;
	Ogre::String		mName;
	Ogre::SceneNode*	mCharacterNode;
	Ogre::Entity*		mCharacterEntity;
	Ogre::String		mCharacterEntityLoc;
	Ogre::String		mCharacterTextureLoc;
	AnimationHelper*	mAnimationHelper;

	//movement variables
	Ogre::Vector3		mHeading;
	float				mRotation; //in degrees
	Ogre::Vector3		mVelocity;
	float				mMaxSpeed;

public:
	Character(int collisionType);
	void Collision(CollisionObject* other) { }

	virtual void Initialize();
	virtual void Startup(){}
	virtual void Shutdown(){}
	virtual bool Update(DWORD timeSinceLastFrame){ return true; }

	bool ValidateMovement();
	void Move();
	void Attack(Character* enemy, int damage);
	void Attack(std::vector<Character*> enemies, int damage, int attackType);
	virtual bool CheckDeath();

	//animation methods
	virtual void AnimateBasedOnMovement(){}
	virtual void SetupAnimations(){}
	
	//movement
	void SetCharacterOrientationToHeading();
	void MoveCharacterForward();
	void MoveCharacterBackward();
	Ogre::Vector3 GetHeading();
	
	void ModifyHealth(int value);
	void SteeringForceMove(float speed);

	//getters
	Ogre::AxisAlignedBox GetBoundingBox() 
	{ 
		if(mCharacterEntity != NULL && mCharacterNode != NULL)
		{
			Ogre::AxisAlignedBox bounds;

			bounds.setMaximumX(mCharacterNode->getPosition().x + bounds.getMaximum().x);
			bounds.setMinimumX(mCharacterNode->getPosition().x + bounds.getMinimum().x);
			bounds.setMaximumZ(mCharacterNode->getPosition().z + bounds.getMaximum().z);
			bounds.setMinimumZ(mCharacterNode->getPosition().z + bounds.getMinimum().z);
			return bounds; 
		}
	}
	int GetAgility()				const { return mAgility; }
	int GetArmor()					const { return mArmor; }
	int GetCurrentHealth()			const { return mCurrentHealth; }
	int GetMaxHealth()				const { return mMaxHealth; }
	float GetMaxSpeed()				const { return mMaxSpeed; }
	Ogre::String GetMesh()			const { return mCharacterEntityLoc; }
	Ogre::String GetName()			const { return mName; }
	Ogre::Vector3 GetPosition()		const { return mCharacterNode->getPosition(); }
	double GetRotation()			const { return mRotation; }
	Ogre::SceneNode* GetSceneNode() const { return mCharacterNode; }
	Ogre::Entity* GetEntity()		const { return mCharacterEntity; }
	int GetStrength()				const { return mStrength; }
	Ogre::String GetTexture()		const { return mCharacterTextureLoc; }
	Ogre::Vector3 GetVelocity()		const { return mVelocity; }
	bool IsActive()					const { return mIsActive; }
	StateManager* GetStateManager()		{ return mCharacterStateManager; }
	
	//setters
	void SetRotation(float value){ mRotation = value; }
	void SetMaxSpeed(float value){ mMaxSpeed = value; }
	void SetVelocity(Ogre::Vector3 value){ mVelocity = value; }
	void SetName(Ogre::String name){ mName = name; }
	void SetTexture(Ogre::String texture){ mCharacterTextureLoc = texture; mCharacterEntity->setMaterialName(texture); }
	void SetMesh(Ogre::String newEntityLoc) { mCharacterEntityLoc = newEntityLoc; mCharacterEntity = GRAPHICSENGINE.GetSceneManager()->createEntity(newEntityLoc); }
	void SetActive(bool value){ mIsActive = value; }
};

#endif

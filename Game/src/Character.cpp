#include "Character.h"

Character::Character(int CollisionType)
	: CollisionObject(CollisionType) 
{
	Initialize();
}

void Character::Initialize()
{
	mCharacterStateManager = new StateManager;
	mCharacterNode = GRAPHICSENGINE.GetSceneManager()->createSceneNode();
	SetActive(false);
}

Ogre::Vector3 Character::GetHeading()
{
	return Ogre::Vector3(
		Ogre::Math::Cos(Ogre::Math::DegreesToRadians(mRotation)),
		0,
		Ogre::Math::Sin(Ogre::Math::DegreesToRadians(mRotation)));
}

void Character::SetCharacterOrientationToHeading()
{
	Ogre::Vector3 mDirection = GetHeading();
	mCharacterNode->resetOrientation();

	Ogre::Vector3 currentHeading = mCharacterNode->getOrientation() * Ogre::Vector3::UNIT_X;
	
	if ((1.0f + currentHeading.dotProduct(mDirection)) < 0.0001f) 
    {
        mCharacterNode->yaw(Ogre::Degree(180));
    }
    else
    {
		currentHeading.y = 0;
		mDirection.y = 0;
		currentHeading.normalise();
		mDirection.normalise();
		Ogre::Quaternion quat = currentHeading.getRotationTo(mDirection);
			
        mCharacterNode->rotate(quat);	
    }

	if(mName == "Attackers")
		mCharacterNode->yaw(Ogre::Degree(270));
	else
		mCharacterNode->yaw(Ogre::Degree(90));
}

void Character::MoveCharacterForward()
{
	mCharacterNode->setPosition(mCharacterNode->getPosition() + (GetHeading()*mMaxSpeed));
}

void Character::MoveCharacterBackward()
{
	mCharacterNode->setPosition(mCharacterNode->getPosition() - (GetHeading()*mMaxSpeed));
}

//Used to move enemies and automated movement of the player.
void Character::SteeringForceMove(float speed)
{
	mCharacterNode->setPosition(mCharacterNode->getPosition() + (GetHeading()*speed));
	SetCharacterOrientationToHeading();
}
void Character::ModifyHealth(int value)
{
	if((mCurrentHealth + value) < mMaxHealth)
		mCurrentHealth += value;
	else
		mCurrentHealth = mMaxHealth;
	CheckDeath();
}

bool Character::CheckDeath()
{
	if(mCurrentHealth <= 0)
	{
		CollisionObject::Shutdown();
		mCharacterNode->setPosition(100000,100000,100000); //temporarily port unit off screen
		std::cout << mName << " has been slain. " << std::endl;
		SetActive(false);
		return true;
	}
	else
		return false;
}

void Character::Attack(Character* enemy, int damage)
{
	std::cout <<GetName()<<" deals "<<damage<<" damage to "<<enemy->GetName() << std::endl;
	enemy->ModifyHealth(-damage);
}

void Character::Attack(std::vector<Character*> enemies, int damage, int attackType)
{
	for(std::vector<Character*>::iterator it = enemies.begin(); it != enemies.end(); it++)
	{
		std::cout <<GetName()<<" deals "<<damage<<" damage to "<<(*it)->GetName() << std::endl;
		(*it)->ModifyHealth(-damage);
	}
}
#include "Guardian.h"
#include "GameEngine.h"
#include "CON_EnemyConstants.h"

Guardian::Guardian()
{
	mName = "Guardian";
	mCharacterEntityLoc = DEFAULT_ENEMY_MESH;
	mCharacterTextureLoc = DEFAULT_ENEMY_TEXTURE;

	Initialize();
}

Guardian::Guardian(Ogre::String name, Ogre::String mesh, Ogre::String texture)
{
	mName = name;
	mCharacterEntityLoc = mesh;
	mCharacterTextureLoc = texture;

	Initialize();
}

void Guardian::Initialize()
{
	static bool initialized = false;
	if(initialized)
		Shutdown();

	mAttackDamage = 1;
	mAttackSpeed = 3000;
	mLastAttackTime = 0;
	mAttackRange = 1;
	mMaxHealth = 10;
	mCurrentHealth = 10;
	

	mSteering = new SteeringBehaviors(this);
	
	if(GRAPHICSENGINE.GetSceneManager()->createEntity(mCharacterEntityLoc))
		mCharacterEntity = GRAPHICSENGINE.GetSceneManager()->createEntity(mCharacterEntityLoc);
	else
		mCharacterEntity = GRAPHICSENGINE.GetSceneManager()->createEntity(DEFAULT_ENEMY_MESH);
	mCharacterEntity->setMaterialName(mCharacterTextureLoc);

	//Attach enemy to level
	tempNode = GRAPHICSENGINE.GetSceneManager()->createSceneNode();
	tempNode->yaw(Ogre::Degree(180));
	tempNode->attachObject(mCharacterEntity);
	mCharacterNode->addChild(tempNode);

	mCharacterNode->scale(0.1f, 0.1f, 0.1f);
	mCharacterNode->setPosition(0, 0, 0);
	
	mCharacterNode->showBoundingBox(false);

	mCharacterEntity->setCastShadows(true);
	mAnimationHelper = new AnimationHelper(mCharacterEntity);

	mRotation = 0;
	SetCharacterOrientationToHeading();

	mMaxSpeed = MINION_MOVE_SPEED;
	mVelocity = Ogre::Vector3::ZERO;
}

void Guardian::Startup()
{
	RegisterCollisionObject();
	if(mCharacterEntity && mCharacterNode)
	{
		//Attach enemy to level
		GRAPHICSENGINE.GetSceneManager()->getRootSceneNode()->addChild(mCharacterNode);

		mSteering->PursuitOn(GAMEENGINE.GetPlayer());
		mCharacterStateManager = new StateManager(WANDER);
		SetActive(true);
	}
}

void Guardian::Shutdown()
{
	delete(mSteering);
	mSteering = NULL;

	GRAPHICSENGINE.GetSceneManager()->getRootSceneNode()->removeChild(mCharacterNode);

	mCharacterNode->detachAllObjects();

	delete(mCharacterEntity);
	delete(mCharacterNode);

	mCharacterEntity = NULL;
	mCharacterNode = NULL;
}

bool Guardian::Update(DWORD timeSinceLastTick)
{	
	switch(mCharacterStateManager->GetCurrentState())
	{
	case WANDER:
		Wander();
		break;
	case PURSUE:
		PursuePlayer();
		break;
	case ATTACK:
		AutoAttack();
		break;
	};
	return true;
	
}

void Guardian::Wander()
{

}

void Guardian::AutoAttack()
{
	mLastAttackTime = GAMEENGINE.GetTotalTimeElapsed();
	Character* obj = GAMEENGINE.GetCollisionManager()->CheckCollisions(this);
	if(obj != NULL)
		Attack(obj, 1);

	mCharacterStateManager->SetCurrentState(PURSUE);
}

void Guardian::PursuePlayer()
{
	Character* obj = GAMEENGINE.GetCollisionManager()->CheckCollisions(this);
	if(obj != NULL)
	{
		if(GAMEENGINE.GetTotalTimeElapsed() >= mAttackSpeed + mLastAttackTime)
		{
			mCharacterStateManager->SetCurrentState(ATTACK);
			return;
		}
		mSteering->SetTarget(GAMEENGINE.GetPlayer()->GetSceneNode()->getPosition());
		if(mSteering->CalculateDistanceToTarget() > ENEMY_CLOSE_DISTANCE)
		{
			HandleMovement();
		}
	} 
	else 
	{
		HandleMovement();
	}
}

void Guardian::HandleMovement()
{
	mSteering->SetTarget(GAMEENGINE.GetPlayer()->GetSceneNode()->getPosition());
	
	float speed = mMaxSpeed;
	mVelocity += mSteering->Calculate();

	if(mVelocity.length() > 0.0001)
	{
		Ogre::Radian temp = mVelocity.angleBetween(Ogre::Vector3::UNIT_X);
		mRotation = (float)temp.valueDegrees();
		
		if(mVelocity.z < 0)
			mRotation = 360 - mRotation;

		while(mRotation > 360)
			mRotation -= 360;

		while(mRotation < 0)
			mRotation += 360;
	}

	SteeringForceMove(speed);
}
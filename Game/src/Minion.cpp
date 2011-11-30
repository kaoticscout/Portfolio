#include "Minion.h"
#include "GameEngine.h"
#include "CON_EnemyConstants.h"

Minion::Minion()
	: Enemy()
{
	mName = "Minion";
	mCharacterEntity = GRAPHICSENGINE.GetSceneManager()->createEntity(DEFAULT_ENEMY_MESH);

	Initialize();
}

Minion::Minion(Ogre::String name, Ogre::String mesh, Ogre::String texture)
	: Enemy()
{
	mName = name;
	
	if(GRAPHICSENGINE.GetSceneManager()->createEntity(mesh))
		mCharacterEntity = GRAPHICSENGINE.GetSceneManager()->createEntity(mesh);
	else
		mCharacterEntity = GRAPHICSENGINE.GetSceneManager()->createEntity(DEFAULT_ENEMY_MESH);

	mCharacterEntityLoc = mesh;
	mCharacterTextureLoc = texture;

	Initialize();
}

void Minion::Initialize()
{
	mMaxHealth = 10;
	mCurrentHealth = 10;
	mAttackDamage = 1;
	mAttackSpeed = 3000;
	mLastAttackTime = 0;
	mAttackRange = 1;

	mSteering = new SteeringBehaviors(this);

	mCharacterNode->scale(.1, .1, .1);
	mCharacterNode->setPosition(100, 100, 0);
	mCharacterNode->showBoundingBox(false);

	mCharacterEntity->setCastShadows(true);
	mAnimationHelper = new AnimationHelper(mCharacterEntity);
	mAnimationHelper->SetTopAnimation("Idle", false);

	mRotation = 0;
	SetCharacterOrientationToHeading();

	mMaxSpeed = MINION_MOVE_SPEED;
	mVelocity = Ogre::Vector3::ZERO;
}
void Minion::Startup()
{
	RegisterCollisionObject();

	//Attach enemy to level
	Ogre::SceneNode* tempNode = GRAPHICSENGINE.GetSceneManager()->createSceneNode();
	
	tempNode->yaw(Ogre::Degree(-270));
	tempNode->attachObject(mCharacterEntity);
	mCharacterNode->addChild(tempNode);
	mCharacterEntity->setMaterialName(mCharacterTextureLoc);

	GRAPHICSENGINE.GetSceneManager()->getRootSceneNode()->addChild(mCharacterNode);
	
	//Set default AI
	mSteering->PursuitOn(GAMEENGINE.GetPlayer());
	mCharacterStateManager = new StateManager(PURSUE);
	SetActive(true);
}

void Minion::Shutdown()
{
	GRAPHICSENGINE.GetSceneManager()->getRootSceneNode()->removeChild(mCharacterNode);
	mCharacterNode->detachAllObjects();

	delete mSteering;
	delete mAnimationHelper;	
	delete mCharacterEntity;
	delete mCharacterNode;
	mSteering = NULL;
	mAnimationHelper = NULL;
	mCharacterEntity = NULL;
	mCharacterNode = NULL;
}

bool Minion::Update(DWORD timeSinceLastTick)
{	
	if(mIsActive)
	{
		switch(mCharacterStateManager->GetCurrentState())
		{
		case PURSUE:
			PursuePlayer();
			break;
		case ATTACK:
			AutoAttack();
			break;
		};
	}

	mAnimationHelper->UpdateAnimations(timeSinceLastTick);
	return true;
}
void Minion::AutoAttack()
{
	mAnimationHelper->SetTopAnimation("Shoot", true);
	std::cout << "Shooting" << std::endl;
	mLastAttackTime = GAMEENGINE.GetTotalTimeElapsed();
	Character* obj = GAMEENGINE.GetCollisionManager()->CheckCollisions(this);
	if(obj != NULL)
		Attack(obj, 1);

	mCharacterStateManager->SetCurrentState(PURSUE);
}
void Minion::PursuePlayer()
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
			HandleMovement();
	} 
	else 
		HandleMovement();

}
void Minion::HandleMovement()
{
	mSteering->SetTarget(GAMEENGINE.GetPlayer()->GetSceneNode()->getPosition());
	
	float speed = mMaxSpeed;
	mVelocity += mSteering->Calculate();
	
	if(mVelocity.squaredLength() > mMaxSpeed*mMaxSpeed)
	{
		mVelocity.normalise();
		mVelocity *= mMaxSpeed;
	}
	
	if(mVelocity.length() > 0.0001)
	{
		Ogre::Radian temp;
		temp = mVelocity.angleBetween(Ogre::Vector3::UNIT_X);
		mRotation = (float)temp.valueDegrees(); 

		if(mVelocity.z < 0)
			mRotation = 360 - mRotation;

		while(mRotation > 360)
			mRotation -= 360;
		while(mRotation < 0)
			mRotation += 360;

		mAnimationHelper->SetTopAnimation("Walk", false);
	}
	else
		mAnimationHelper->SetTopAnimation("Idle", false);

	SteeringForceMove(speed);
}
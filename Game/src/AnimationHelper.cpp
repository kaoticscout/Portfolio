#include "AnimationHelper.h"

void AnimationHelper::UpdateAnimations(DWORD deltaTime)
{		
	Ogre::Real baseAnimSpeed = 1;
	Ogre::Real topAnimSpeed = 1;

	Ogre::Real delta = (Ogre::Real)deltaTime; //use Real as DWORD does not have appropriate precision
	delta /= 1000;// convert milliseconds into seconds
	mAnimationTimePlayed += delta;
	if(mAnimationStateBottom != NULL)
		mAnimationStateBottom->addTime(delta * baseAnimSpeed);
	if(mAnimationStateTop != NULL)
		mAnimationStateTop->addTime(delta * topAnimSpeed);

	if(mAnimationTimePlayed >= mAnimationStateTop->getLength())
		mAnimationPlaying = false;
}

void AnimationHelper::SetTopAnimation(Ogre::String text, bool playUntilFinished = false)
{
	if(!mAnimationPlaying)
	{
		if(mAnimationStateTop != NULL)
		{
			mAnimationStateTop->setLoop(false);
			mAnimationStateTop->setEnabled(false);
		}
		try
		{	
			mAnimationPlaying = playUntilFinished;
			mCurrentAnimation = text;
			mAnimationTimePlayed = 0;
			mAnimationStateTop = mCharacterMesh->getAnimationState(text);
			mAnimationStateTop->setLoop(true);
			mAnimationStateTop->setEnabled(true);
		}
		catch(...)
		{
			std::cout << "Top animation (" << text << ") missing." << std::endl;
		}
	}
}

void AnimationHelper::SetBottomAnimation(Ogre::String text)
{
	if(mAnimationStateBottom != NULL)
	{
		mAnimationStateBottom->setLoop(false);
		mAnimationStateBottom->setEnabled(false);
	}

	try
	{
		mCurrentAnimation = text;
		mAnimationStateBottom = mCharacterMesh->getAnimationState(text);
		mAnimationStateBottom->setLoop(true);
		mAnimationStateBottom->setEnabled(true);
	}
	catch(...)
	{
		std::cout << "Bottom animation missing." << std::endl;
	}
}
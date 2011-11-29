#ifndef ANIMATIONHELPER_H
#define ANIMATIONHELPER_H

#include "OgrePrerequisites.h"
#include "Ogre.h"

//Animation Constants
const int NUM_ANIMS = 13;	// number of animations the character has
const Ogre::Real ANIM_FADE_SPEED = 7.5f;	// animation crossfade speed in % of full weight per second
const Ogre::Real JUMP_ACCEL = 30.0f;		// character jump acceleration in upward units per squared second
const Ogre::Real GRAVITY = 90.0f;			// gravity in downward units per squared second

class AnimationHelper
{
private:
	Ogre::String			mCurrentAnimation;
	Ogre::Entity*			mCharacterMesh;
	Ogre::AnimationState*	mAnimationStateBottom;	// The current animation state of the object's bottom half
	Ogre::AnimationState*	mAnimationStateTop;		// The current animation state of the object's top half
	Ogre::Real				mAnimationTimePlayed;
	bool					mAnimationPlaying;

protected:
public:
	void UpdateAnimations(DWORD deltaTime);

	AnimationHelper(Ogre::Entity* charMesh)
	{
		mCharacterMesh = charMesh;

		mAnimationStateBottom = NULL;	
		mAnimationStateTop = NULL;	
		mAnimationPlaying = false;
		mAnimationTimePlayed = 0;
	}

	void SetTopAnimation(Ogre::String text, bool playUntilFinished);
	void SetBottomAnimation(Ogre::String text);
	inline Ogre::String GetTopAnimation() { return mCurrentAnimation; }
	inline Ogre::String GetBottomAnimation() { return mCurrentAnimation; }
	void GetAnimations() 
	{ 
		Ogre::AnimationStateSet* set = mCharacterMesh->getAllAnimationStates(); 
		Ogre::AnimationStateIterator iter=set->getAnimationStateIterator();
		while(iter.hasMoreElements())
		{
         std::cout<<iter.getNext()->getAnimationName()<<std::endl;
		}
	}
};

#endif

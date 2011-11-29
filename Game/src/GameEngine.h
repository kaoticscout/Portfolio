#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "CollisionManager.h"
#include "GUIManager.h"
#include "LevelManager.h"
#include "OIS.h"
#include "Player.h"
//#include "RegisteredFrameListener.h"
#include "StateManager.h"

#define GAMEENGINE GameEngine::Instance()

class GameEngine
	: public OIS::KeyListener, 
	  public OIS::MouseListener
{
private:
	CollisionManager*		mCollisionManager;
	LevelManager*			mLevelManager;
	GUIManager*				mGUIManager;
	StateManager*			mStateManager;

	Player*					mPlayer;
	int						mCurrentState;   //SHOULDNT NEED THIS, TAKEN CARE OF WITH STATEMANAGER
	Ogre::Real				mTotalGameTime;

	//OIS Input devices
	OIS::InputManager* 		mInputManager;
	OIS::Mouse*    			mMouse;
	OIS::Keyboard* 			mKeyboard;
	
	std::vector<GameComponent*>  mGameComponents;

	void HandleInput();
	void HandleGameLogic(DWORD timeSinceLastTick);
	void InitializeResources();
	void LoadPlayer();
	void SetupInputDevices();
	void StateChanged();
public:
	void Startup();
	void Shutdown();
	void ShutdownInput();
	bool Update(DWORD timeSinceLastFrame);

	//input
	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased( const OIS::KeyEvent &arg );
	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	
	//singleton
	static GameEngine &Instance()
	{
		static GameEngine instance;
		return instance;
	}

	//getters
	Player*				GetPlayer()				const { return mPlayer; }
	CollisionManager*	GetCollisionManager()	const { return mCollisionManager; }
	GUIManager*			GetGUIManager()			const { return mGUIManager; }
	StateManager*		GetStateManager()		const { return mStateManager; }
	LevelManager*		GetLevelManager()		const { return mLevelManager;}

	OIS::InputManager*	GetInputManager()		const { return mInputManager; }
	OIS::Keyboard*		GetKeyboard()			const { return mKeyboard; }
	OIS::Mouse*			GetMouse()				const { return mMouse; }
	float			    GetTotalTimeElapsed()	const { return (float)mTotalGameTime; }
};

#endif


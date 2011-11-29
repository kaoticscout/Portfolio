#include "GameEngine.h"
#include "GraphicsEngine.h"
#include "CON_GameConstants.h"

void GameEngine::Startup()
{
	InitializeResources();

	mStateManager = new StateManager(INITIALIZING_GAME);
	mCollisionManager = new CollisionManager();
	SetupInputDevices();
	mGUIManager = new GUIManager();

	mGameComponents.push_back(mGUIManager);
	mStateManager->SetCurrentState(LOGO);
}

void GameEngine::Shutdown()
{
	delete mGUIManager;
	//delete mPlayer;
	delete mCollisionManager;
	delete mStateManager;

	ShutdownInput();
	InitializeResources();
}

void GameEngine::InitializeResources()
{
	mCollisionManager = NULL;
	mGUIManager = NULL;
	mLevelManager = NULL;
	mPlayer = NULL;
	mStateManager = NULL;
	
	mCurrentState = 0;
}

void GameEngine::SetupInputDevices()
{
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	GRAPHICSENGINE.GetRenderWindow()->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = OIS::InputManager::createInputSystem( pl );

	// Create all devices
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true ));
	
	mKeyboard->setEventCallback(this);
	mMouse->setEventCallback(this);
}

void GameEngine::ShutdownInput()
{
	if( mInputManager )
	{
		if (mMouse) mInputManager->destroyInputObject( mMouse );
		if (mKeyboard) mInputManager->destroyInputObject( mKeyboard );

		OIS::InputManager::destroyInputSystem(mInputManager);
		mInputManager = NULL;
	}
}

void GameEngine::LoadPlayer()
{
	//TODO Implement this with xml
	mPlayer = new Player();
	mGameComponents.push_back(mPlayer);

}

bool GameEngine::Update(DWORD timeSinceLastFrame)
{
	//std::cout<<"TimeSinceLastFrame: "<<timeSinceLastFrame<<std::endl;
	mTotalGameTime += timeSinceLastFrame;

	mKeyboard->capture();
	mMouse->capture();

	HandleInput();		
	HandleGameLogic(timeSinceLastFrame);

	if(mCurrentState != mStateManager->GetCurrentState())
		StateChanged();

	return true;
}

void GameEngine::HandleInput()
{
	//ONLY FOR DEBUGGING
	GRAPHICSENGINE.ToggleConsoleWindow();
}

void GameEngine::HandleGameLogic(DWORD timeSinceLastTick)
{
	if(mStateManager->GetCurrentState() != PAUSE_GAME)
	{
		for(int i = 0; i<mGameComponents.size(); i++)
			mGameComponents.at(i)->Update(timeSinceLastTick);
	}
	else
		mGUIManager->Update(timeSinceLastTick);
}

void GameEngine::StateChanged()
{
	if(mStateManager->GetCurrentState() == LOADING_LEVEL)
	{
		LoadPlayer();
		
		mLevelManager = new LevelManager();
		mLevelManager->LoadNextLevel();
		mGameComponents.push_back(mLevelManager);

		mStateManager->SetCurrentState((int)GAME_STATE::PLAYING_LEVEL);
	}
	else if(mStateManager->GetCurrentState() == LOGO)
	{
	}
	else if(mStateManager->GetCurrentState() == MAIN_MENU)
	{
	}
	else if(mStateManager->GetCurrentState() == LOGGING_OUT)
	{
		mLevelManager->Shutdown();
		delete mPlayer;
		delete mLevelManager;
		mGameComponents.pop_back(); //pop LevelManager
		mGameComponents.pop_back(); //pop Player

		mStateManager->SetCurrentState((int)GAME_STATE::CHARACTER_SELECTION);
	}
	
	mCurrentState = mStateManager->GetCurrentState();
}

bool GameEngine::keyPressed( const OIS::KeyEvent &arg )
{
	//Various escape functionality
	if(arg.key == OIS::KC_ESCAPE)
	{
		int currentState = mStateManager->GetCurrentState();
		if (currentState == ((int)GAME_STATE::PLAYING_LEVEL) || currentState == ((int)GAME_STATE::PAUSE_GAME))
		{
			if (currentState != (int)GAME_STATE::PAUSE_GAME)
				mStateManager->SetCurrentState((int)GAME_STATE::PAUSE_GAME);
			else
				mStateManager->SetCurrentState(mStateManager->GetPreviousState());
		}
		else if (currentState == ((int)GAME_STATE::CHARACTER_SELECTION))
		{
			mStateManager->SetCurrentState((int)GAME_STATE::MAIN_MENU);
		}
		else if (currentState == ((int)GAME_STATE::MAIN_MENU) || currentState == ((int)GAME_STATE::LOGO))
		{
			mStateManager->SetCurrentState((int)GAME_STATE::SHUTTING_DOWN);
		}
	}

	mGUIManager->keyPressed(arg);
	return true;
}

bool GameEngine::keyReleased( const OIS::KeyEvent &arg )
{
	return true;
}

bool GameEngine::mouseMoved( const OIS::MouseEvent &arg )
{
	return true;
}
bool GameEngine::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	mGUIManager->mousePressed(arg, id);
	return true;
}
bool GameEngine::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	mGUIManager->mouseReleased(arg, id);
	return true;
}
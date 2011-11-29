#include <iostream>
#include <sstream>
#include "GraphicsEngine.h"
#include "GameEngine.h"

GraphicsEngine::GraphicsEngine() 
{
	Initialize();
}

GraphicsEngine::~GraphicsEngine()
{

}

void GraphicsEngine::Initialize()
{
	root = NULL;
	window = NULL;
	engineManagerRunning = true;
	mSceneManager = NULL;
	mCamera = NULL;
	mViewport = NULL;
}

bool GraphicsEngine::Startup(const std::string pluginFileName, const std::string configFileName, const std::string logFileName)
{
	//Seed the random number generator
	srand( (unsigned)time( NULL ) );

    root = new Ogre::Root(pluginFileName);

	SetupDebugConsole();

	SetupResources();
	LoadResources();

	if (!Configure()) return false;

	TextureManager::getSingleton().setDefaultNumMipmaps(5);

	mSceneManager = root->createSceneManager("DefaultSceneManager");

	//creates a camera with the default camera settings
	Ogre::Camera* mCamera = mSceneManager->createCamera("MainCamera");

	//initial camera settings
	mCamera->setPosition(Ogre::Vector3(0,10,-30));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
	mCamera->setNearClipDistance(5);

	mSceneManager->createEntity("SinbadSword1", "Sword.mesh");
	mSceneManager->createEntity("SinbadSword2", "Sword.mesh");

	//add the camera to the viewport
	mViewport = window->addViewport(mCamera);

	root->addFrameListener(this);
	WindowEventUtilities::addWindowEventListener(window, this);

	return true;
}


void GraphicsEngine::Shutdown()
{
	if(mViewport != NULL)
		window->removeViewport(mViewport->getZOrder());
	//if(mSceneManager != NULL)
		//root->destroySceneManager(mSceneManager);
	GAMEENGINE.ShutdownInput();
	root->removeFrameListener(this);
	WindowEventUtilities::removeWindowEventListener(window, this);

	Initialize();
}

void GraphicsEngine::StartRenderLoop()
{
	if (root != NULL)
		root->startRendering();
}

void GraphicsEngine::SetupResources()
{
	Ogre::ConfigFile cf;
	#ifdef _DEBUG
		cf.load("resources_d.cfg");
	#else
		cf.load("resources.cfg");
	#endif

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
}

void GraphicsEngine::SetupDebugConsole()
{
	static const WORD MAX_CONSOLE_LINES = 500;
    int hConHandle;
    long lStdHandle;
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    FILE *fp;

    // allocate a console for this app
    AllocConsole();
	
	// set the screen buffer to be big enough to let us scroll text
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
    coninfo.dwSize.Y = MAX_CONSOLE_LINES;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),
    coninfo.dwSize);

    //redirect unbuffered STDOUT to the console
    lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    fp = _fdopen( hConHandle, "w" );
    *stdout = *fp;
    setvbuf( stdout, NULL, _IONBF, 0 );

    // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
    // point to console as well
    std::ios::sync_with_stdio();

	SetConsoleTitle("Processes Game Debug Console");

	std::cout<<"Console Created"<<std::endl;
	std::cout<<"Hit <Escape> to exit game."<<std::endl;
}

bool GraphicsEngine::Configure()
{
	if (root->showConfigDialog())
		window = root->initialise( true, "OGRE" );
	else
		return false;
	return true;
}

void GraphicsEngine::windowClosed(RenderWindow* rw)
{
	//GAMEENGINE.ShutdownInput();
	GAMEENGINE.GetStateManager()->SetCurrentState(SHUTTING_DOWN);
	this->StopRenderLoop();
}

bool GraphicsEngine::frameStarted(const FrameEvent& evt)
{	
	return engineManagerRunning;
}

void GraphicsEngine::ToggleConsoleWindow()
{
	static bool isTabDown = false;
	static bool isVisible = true;

	//Toggles Console Window (TAB Key)
	if(GAMEENGINE.GetKeyboard()->isKeyDown(OIS::KC_TAB))
	{
		if(isTabDown != true)
		{
			HWND hWnd = GetConsoleWindow();
			if(isVisible)
			{
				ShowWindow( hWnd, SW_HIDE );
				isVisible = false;
			}
			else
			{
				ShowWindow( hWnd, SW_SHOW );
				isVisible = true;
			}
		}
		isTabDown = true;
	}
	else
		isTabDown = false;
}


void GraphicsEngine::LoadResources()
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
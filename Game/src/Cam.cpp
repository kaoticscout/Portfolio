#include "Cam.h"
#include "GameEngine.h"
#include "GraphicsEngine.h"
#include "CON_GameConstants.h"

Cam::Cam()
{
	std::cout << "Creating Camera." << std::endl;
	mCamera = GRAPHICSENGINE.GetSceneManager()->createCamera("MainCamera");

	//initial camera settings
	mCamera->setPosition(Ogre::Vector3(0,-30,-10));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
	mCamera->setNearClipDistance(5);
}

Cam::~Cam()
{
}

void Cam::Startup()
{
	RegisteredFrameListener::Startup();
}

void Cam::Shutdown()
{
	RegisteredFrameListener::Shutdown();
}

bool Cam::Update(const FrameEvent& evt)
{
	mCamera->lookAt(Ogre::Vector3(GAMEENGINE.GetPlayer()->GetSceneNode()->getPosition()));
	return true;
}


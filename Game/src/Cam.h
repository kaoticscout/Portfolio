#ifndef CAM_H
#define CAM_H
#include <OgreCamera.h>
#include "RegisteredFrameListener.h"

class Cam
	: public RegisteredFrameListener
{
private:
	Ogre::Camera*	mCamera;
protected:
public:
	Cam();
	~Cam();
	void Startup();
	void Shutdown();
	bool Update(const FrameEvent& evt);
	Ogre::Camera* GetCamera(){ return mCamera; }
};
#endif
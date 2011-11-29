#ifndef REGISTEREDFRAMELISTENER_H
#define REGISTEREDFRAMELISTENER_H

#include "Ogre.h"
#include "GraphicsEngine.h"
#include "CON_GameConstants.h"

class RegisteredFrameListener :
	public FrameListener
{
protected:
	FrameEvent GetFixedFrameEvent(const FrameEvent& evt)
	{
		FrameEvent fixed;
		fixed.timeSinceLastFrame = evt.timeSinceLastFrame > MAX_FRAME_TIME ?
			MAX_FRAME_TIME:
			evt.timeSinceLastFrame;
		return fixed;
	}
	bool isStarted;

public:
	RegisteredFrameListener()
	{
		isStarted = false;
		GRAPHICSENGINE.GetRoot()->addFrameListener(this);
	}
	
	virtual ~RegisteredFrameListener()
	{
		if (GRAPHICSENGINE.GetRoot() != NULL)
			GRAPHICSENGINE.GetRoot()->removeFrameListener(this);
	}

	void Startup()
	{
		isStarted = true;
	}

	void Shutdown()
	{
		isStarted = false;
	}

	bool frameStarted(const FrameEvent& evt)
	{
		if (this->isStarted)
			return Update(GetFixedFrameEvent(evt));

		return true;
	}

	bool frameEnded(const FrameEvent& evt)
	{
		 if (this->isStarted)			
			return FrameEnded(GetFixedFrameEvent(evt));

		 return true;
	}
	
	virtual bool FrameEnded(const FrameEvent& evt) {return true;}

	virtual bool Update(const FrameEvent& evt) { return true; }
	bool IsStarted() const { return isStarted; }
};

#endif /* PERSISTENTFRAMELISTENER_H_ */


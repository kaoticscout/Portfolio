/*
 * 		Engine.h
 *		Engine initialization code written by Matthew Casperson
 *		Email:		matthewcasperson@gmail.com
 *		Website:	http://www.brighthub.com/hubfolio/matthew-casperson.aspx
 */

#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H

#include "memory"
#include "string"
#include "map"

#include "Ogre.h"

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>

#include "ResourceLocationDefinition.h"

// define this to make life a little easier
#define GRAPHICSENGINE GraphicsEngine::Instance()

// All OGRE objects are in the Ogre namespace.
using namespace Ogre;

class GraphicsEngine :
	public WindowEventListener,
	public FrameListener
{
private:
	void SetupResources();
	void SetupDebugConsole();
	bool Configure();
	void Initialize();
	void LoadResources();

	//Graphics 
	SceneManager*						mSceneManager;
	Camera*								mCamera;
	Viewport*							mViewport;
	Ogre::Root*							root;
	RenderWindow* 						window;
	bool 								engineManagerRunning;
	ResourceLocationDefinitionVector 	resourceLocationDefinitionVector;

public:
	GraphicsEngine();
	~GraphicsEngine();

	static GraphicsEngine & Instance()
	{
		static GraphicsEngine instance;
		return instance;
	}

	bool Startup(const std::string pluginFileName, const std::string configFileName, const std::string logFileName);
	void Shutdown();
	void ToggleConsoleWindow();

	//Getters and setters
	RenderWindow *	GetRenderWindow()	const { return window; }
	Root *			GetRoot()			const { return root; }
	Viewport*		GetViewport()		const { return mViewport; }
	SceneManager*	GetSceneManager()	const { return mSceneManager; }
	Camera*			GetCamera()			const { return mCamera; }

	void StartRenderLoop();
	void StopRenderLoop() { engineManagerRunning = false; }
	void AddNewResourceLocation(const ResourceLocationDefinition& definition) { resourceLocationDefinitionVector.push_back(definition); }
	void windowClosed(RenderWindow* rw);
	void windowResized(RenderWindow* rw) {};
	bool frameStarted(const FrameEvent& evt);
};

#endif 

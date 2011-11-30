#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include "fmod.hpp"

#define SOUNDENGINE SoundEngine::Instance()
class SoundEngine
{
protected:
	FMOD::System*			mSystem;
	FMOD_RESULT				mResult;
	unsigned int			mVersion;
	int						mNumDrivers;
	FMOD_SPEAKERMODE		mSpeakerMode;
	FMOD_CAPS				mCaps;
	char					mName[256];
	FMOD::Channel*			mChannel;
	FMOD::Sound*			mSound;

public:
	void Startup();
	void Shutdown();
	void Update();
	void Play_Sound(const char* fileName);
	void Play_Music(const char* fileName);
		
	//singleton
	static SoundEngine &Instance()
	{
		static SoundEngine instance;
		return instance;
	}
};

#endif
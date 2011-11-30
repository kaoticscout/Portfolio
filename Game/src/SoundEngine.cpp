#include "SoundEngine.h"
#include <iostream>

void SoundEngine::Startup()
{
	mResult = FMOD::System_Create(&mSystem);
	mResult = mSystem->getVersion(&mVersion);
	if(mVersion < FMOD_VERSION)
	{
		std::cout << "FMOD Error! Version is too old." << std::endl;
		return;
	}

	mResult = mSystem->getNumDrivers(&mNumDrivers);
	if(mNumDrivers == 0)
	{
		mResult = mSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
	}
	else
	{
		mResult = mSystem->getDriverCaps(0, &mCaps, 0, &mSpeakerMode);
		mResult = mSystem->setSpeakerMode(mSpeakerMode);
		if(mCaps && FMOD_CAPS_HARDWARE_EMULATED)
		{
			mResult = mSystem->setDSPBufferSize(1024, 10);
		}

		mResult = mSystem->getDriverInfo(0, mName, 256, 0);
		if(strstr(mName, "SigmaTel"))
		{
			mResult = mSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
		}
	}

	mResult = mSystem->init(100, FMOD_INIT_NORMAL, 0);
	if(mResult == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		mResult = mSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		mResult = mSystem->init(100, FMOD_INIT_NORMAL, 0);
	}

	std::cout << "Sound Engine Initialized properly." << std::endl;
}

void SoundEngine::Shutdown()
{
	mResult = mSystem->release();
}

void SoundEngine::Update()
{
	mSystem->update();
}

void SoundEngine::Play_Music(const char* fileName)
{    
	mResult = mSystem->createSound(fileName, FMOD_LOOP_NORMAL, 0, &mSound);
	mResult = mSystem->playSound(FMOD_CHANNEL_FREE, mSound, 0, &mChannel);
}

void SoundEngine::Play_Sound(const char* fileName)
{    
	mResult = mSystem->createSound(fileName, FMOD_LOOP_OFF, 0, &mSound);
	mResult = mSystem->playSound(FMOD_CHANNEL_FREE, mSound, 0, &mChannel);
}

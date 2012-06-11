///////////////////////////////////////////////////////////////////////////////
// Filename: soundclass.h
///////////////////////////////////////////////////////////////////////////////
#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_
/////////////
// LINKING //
/////////////



//////////////
// INCLUDES //
//////////////
#include <windows.h>
///////////////////////////////////////////////////////////////////////////////
// Class name: SoundClass
// This is a wrapper for FMOD, an outstanding 3rd party Audio Library
///////////////////////////////////////////////////////////////////////////////

#include "fmod.hpp"
#include "SoundClass.h"
#include "fmod_errors.h"
#include <vector>

#define MAX_CHANNEL_NUM 32

class AudioClass
{
private:

public:
	
	AudioClass(const AudioClass&);
	~AudioClass();
	static AudioClass* GetInstance();
	bool Initialize(HWND);
	void Shutdown();
	void UpdateFrame();// call every frame
	void PlaySound(std::string tag);// called by upper level
	void PlayBGM(std::string tag);
private:
	void InitializeResource();
	void ERRCHECK(FMOD_RESULT result);
	AudioClass();

// data
private:
	static AudioClass* m_instance;

	FMOD::System* m_system;
	std::vector<FMOD::Channel*> m_channelList;
	std::vector<SoundClass*> m_soundList;
	FMOD::Channel* m_bgmChannel;


};

#endif
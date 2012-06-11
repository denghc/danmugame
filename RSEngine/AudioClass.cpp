///////////////////////////////////////////////////////////////////////////////
// Filename: soundclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "audioclass.h"
AudioClass::AudioClass()
{
	
}
AudioClass::AudioClass(const AudioClass& other)
{
}


AudioClass::~AudioClass()
{
}

void AudioClass::UpdateFrame()
{
	m_system->update();
}

bool AudioClass::Initialize(HWND hwnd)
{
	FMOD_RESULT result;
	result = FMOD::System_Create(&m_system);
	ERRCHECK(result);

	result = m_system->init(MAX_CHANNEL_NUM, FMOD_INIT_NORMAL, 0);
	ERRCHECK(result);

	InitializeResource();

	m_bgmChannel = 0;
	return true;
}

void AudioClass::Shutdown()
{
	// We do not have to care about channels, FMOD will care about it for us.
	for (std::vector<SoundClass*>::iterator iter = m_soundList.begin(); iter != m_soundList.end(); ++iter)
	{
		(*iter)->Release();
	}

	return;
}

void AudioClass::ERRCHECK(FMOD_RESULT result)
{
	//const char* cc = FMOD_ErrorString(result);
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}
}

void AudioClass::InitializeResource()
{
	FMOD_RESULT result;
	FMOD::Sound* snd;
	
	result = m_system->createSound("Resource\\explode.wav", FMOD_HARDWARE, 0, &snd);
	ERRCHECK(result);
	snd->setMode(FMOD_LOOP_OFF);
	SoundClass* sc = new SoundClass();
	sc->Initialize(snd, "explode");
	m_soundList.push_back(sc);

	

}

AudioClass* AudioClass::m_instance = 0;

AudioClass* AudioClass::GetInstance()
{
	if (m_instance == 0)
		m_instance = new AudioClass();
	return m_instance;
}

void AudioClass::PlayBGM(std::string tag)
{
	// bgm is in loop and can be changed
	// so we must have more control over bgm
	for (std::vector<SoundClass*>::iterator iter = m_soundList.begin(); iter != m_soundList.end(); ++iter)
	{
		if ((*iter)->NameEqual(tag))
		{
			FMOD::Sound* snd = (*iter)->GetSound();
			m_system->playSound(FMOD_CHANNEL_FREE, snd, false, 0);
			break;		
		}
	}
}

void AudioClass::PlaySound(std::string tag)
{
	if (m_bgmChannel != 0)
	{
		m_bgmChannel->stop();
	}
	for (std::vector<SoundClass*>::iterator iter = m_soundList.begin(); iter != m_soundList.end(); ++iter)
	{
		if ((*iter)->NameEqual(tag))
		{
			FMOD::Sound* snd = (*iter)->GetSound();
			m_system->playSound(FMOD_CHANNEL_FREE, snd, true, &m_bgmChannel);
			break;		
		}
	}

	m_bgmChannel->setLoopCount(-1);
	m_bgmChannel->setPaused(false);
}
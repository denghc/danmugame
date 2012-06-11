#pragma once
// a wrapper for FMOD::Sound
// attach a tag into the sound
#include <string>
#include "fmod.hpp"
#include "fmod_errors.h"


class SoundClass
{
public:
	SoundClass(void);
	~SoundClass(void);

public:
	void Initialize(FMOD::Sound* asnd, std::string tag);
	bool NameEqual(std::string tname);
	void Release();
	FMOD::Sound* GetSound();
	bool IsStream();

private:
	bool m_isStream;
	std::string m_tag;
	FMOD::Sound* m_sound;
};


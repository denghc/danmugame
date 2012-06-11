#include "SoundClass.h"


SoundClass::SoundClass(void)
{
}


SoundClass::~SoundClass(void)
{
}

void SoundClass::Release()
{
	m_sound->release();
}

bool SoundClass::NameEqual(std::string tname)
{
	if (tname.compare(m_tag) == 0)
		return true;
	return false;
}

FMOD::Sound* SoundClass::GetSound()
{
	return m_sound;
}

void SoundClass::Initialize(FMOD::Sound* asnd, std::string tag)
{
	m_tag = tag;
	m_sound = asnd;
}

bool SoundClass::IsStream()
{
	return m_isStream;
}
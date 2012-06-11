#include "SoundManager.h"
#include "AudioClass.h"

SoundManager::SoundManager(void)
{
}


SoundManager::~SoundManager(void)
{
}


void SoundManager::SMPlaySound(std::string tag)
{
	AudioClass* ac = AudioClass::GetInstance();
}

void SoundManager::SMPlayBGM(std::string tag)
{

}
#pragma once

// upper level sound manager
// we only handle one shot sound, that is, the sound plays once and stop, and bgm sound
// SoundManager assumes that 
#include <string>
class SoundManager
{
public:
	SoundManager(void);
	~SoundManager(void);

	void SMPlaySound(std::string tag);
	void SMPlayBGM(std::string tag);
};


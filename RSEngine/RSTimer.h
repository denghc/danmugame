#pragma once

class RSTimer
{
public:
	RSTimer(void);
	~RSTimer(void);

public:
	// reset the timer
	void Reset();
	void SetGameSpeed(unsigned int gameSpeed);
	float GetInterpolation();
	unsigned int GetGameSpeed();
	// check if game state updating is needed
	bool NeedUpdate();

	unsigned int GetTotalFrame();
	// just touch the timer to calc real fps
	void RenderTouch();
	float GetFPS();
	// check if rendering is needed

	/*
		redering as often as possible
	*/

private:
	unsigned int m_skipTick;
	unsigned int m_nextGameTick;
	unsigned int m_gameSpeed;
	unsigned int m_sysStartTime;
	unsigned int m_lastTime;
	unsigned int m_totFrame;
	float m_fps;
	unsigned int m_lastRenderTick;

};


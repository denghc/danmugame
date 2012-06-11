#include "RSTimer.h"
#include <windows.h>

RSTimer::RSTimer(void)
{
	Reset();
}


RSTimer::~RSTimer(void)
{
	
}

void RSTimer::Reset()
{
	m_nextGameTick = GetTickCount();
	m_lastRenderTick = m_nextGameTick;
	m_totFrame = 0;

}


void RSTimer::SetGameSpeed(unsigned int gameSpeed)
{
	m_gameSpeed = gameSpeed;
	m_skipTick = 1000 / m_gameSpeed;
}

unsigned int RSTimer::GetGameSpeed()
{
	return m_gameSpeed;
}

bool RSTimer::NeedUpdate()
{
	if (GetTickCount() > m_nextGameTick)
	{
		m_nextGameTick += m_skipTick;
		++m_totFrame;
		return true;
	}
	return false;
}

float RSTimer::GetInterpolation()
{
	return float( GetTickCount() + m_skipTick - m_nextGameTick )
		/ float( m_skipTick );
}

unsigned int RSTimer::GetTotalFrame()
{
	return m_totFrame;
}

float RSTimer::GetFPS()
{
	return m_fps;
}

void RSTimer::RenderTouch()
{
	unsigned int tmp = GetTickCount();
	m_fps = 1000 / (float)(tmp - m_lastRenderTick);
	m_lastRenderTick = tmp;
}
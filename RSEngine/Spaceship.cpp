#include "Spaceship.h"
#include "GameLogic.h"
//#include <d3dx10math.h>
#include "math.h"

Spaceship::Spaceship(void)
{
	xStep = 3.0f;
	yStep = 3.0f;
	m_bbx = -150;
	m_bby = 0;
}

void Spaceship::Reset()
{
	m_bbx = -150;
	m_bby = 0;
}

Spaceship::~Spaceship(void)
{
}

void Spaceship::Move(int moveCode)
{
	switch (moveCode)
	{
	case MOVE_LEFT_CODE:
		m_bbx -= xStep;
		break;
	case MOVE_RIGHT_CODE:
		m_bbx += xStep;
		break;
	case MOVE_UP_CODE:
		m_bby -= yStep;
		break;
	case MOVE_DOWN_CODE:
		m_bby += yStep;
		break;
	}

	//saturation
	if (m_bbx < - 385)
		m_bbx = -385;
	if (m_bbx > 385)
		m_bbx = 385;
	if (m_bby < - 285)
		m_bby = -285;
	if (m_bby > 290)
		m_bby = 290;
}

void Spaceship::GetXYZ(float* x, float* y, float*z)
{
	float cameraAngle = GameLogic::GetInstance()->GetCameraAngle();
	*x = m_bbx;
	*y = -m_bby * cos(cameraAngle);
	*z = m_bby * sin(cameraAngle);
}

void Spaceship::GetScrXY(float* x, float* y)
{
	*x = m_bbx;
	*y = m_bby;
}
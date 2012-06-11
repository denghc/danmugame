#pragma once
#define MOVE_UP_CODE 0
#define MOVE_DOWN_CODE 1
#define MOVE_LEFT_CODE 2
#define MOVE_RIGHT_CODE 3

class Spaceship
{
public:
	Spaceship(void);
	~Spaceship(void);

public:
	void Move(int moveCode);
	void GetXYZ(float* x, float* y, float*z); // return x, y, z in 3d space
	void GetScrXY(float* x, float* y);
	void Reset();
private:
	float xStep, yStep;
	float m_bbx, m_bby; // coordinate in billboard
};


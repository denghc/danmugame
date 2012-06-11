#pragma once

class BulletSystem;
class UIManager;
class Spaceship;
class InputManager;
class GameGlobalData;
class TextClass;
class InputClass;
class CameraClass;
class StateMachine;
class GameLogic
{
public:

	~GameLogic(void);
	static GameLogic* GetInstance();
	// clac the next key frame
	void UpdateFrame(unsigned int, unsigned int);

	void HandleInput();
	
	void ResetGame();
	// update interpolate
	void UpdateInterpolate(float interpoloate);
	float GetCameraAngle();
	void RotateCamera(float angle);

private:
	GameLogic(void);
	static GameLogic* m_instance;
	

private:
	BulletSystem* m_bulletSys;
	Spaceship* m_spaceship;
	InputManager* m_inputMgr;
	UIManager* m_uiMgr;
	GameGlobalData* m_gameGD;
	CameraClass* m_camera;

// for input mgr
public:
	void InitInputMgr(InputClass* ic);
private:
	
// for ui mgr
public:
	void InitUIMgr(TextClass* tc);

public:
	void InitCamera(CameraClass* cc);
	float GetCameraRadius();
//bullet
public:
	int FillBulletBuffer(void* vb);


public:
	int NeedBlur();


// game data
public:
	// for outer class getting data
	void GetSpaceshipXYZ(float*x, float* y, float* z);
private:
	int m_totFrame;
	float m_totTime;
	unsigned int m_score;
	float m_cameraAngle;// along the axi
	unsigned int m_screenBullet;// totnumber of bullets in screen
	float m_cameraRadius;
	StateMachine* m_stateMachine;
	bool m_isDead;
	int m_lastFrame;
	int m_niceMove;
	int m_niceMoveFrameCountDown;
	int m_noUseFrameCountDown;
	int m_deadLastFrameCountDown;
};

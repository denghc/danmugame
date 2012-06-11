#pragma once
/*
	this class records and handles the game state
*/
// game states
#define SM_MENU 0X101
#define SM_GAME 0X102
#define SM_EVAL 0X103


class StateMachine
{
public:
	StateMachine(void);
	~StateMachine(void);

	int GetCurrentState();
	void Transit(int targetState);
private:
	int m_curState; // current game state

};


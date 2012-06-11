#include "StateMachine.h"


StateMachine::StateMachine(void)
{
	m_curState = SM_MENU;
}


StateMachine::~StateMachine(void)
{
}

int StateMachine::GetCurrentState()
{
	return m_curState;
}

void StateMachine::Transit(int targetState)
{
	switch (m_curState)
	{
	case SM_MENU:
		//TODO
		break;
	case SM_GAME:
		//TODO
		break;
	case SM_EVAL:
		//TODO
		break;
	}

	m_curState = targetState;

	switch (m_curState)
	{
	case SM_MENU:
		//TODO
		break;
	case SM_GAME:
		//TODO
		break;
	case SM_EVAL:
		//TODO
		break;
	}
}
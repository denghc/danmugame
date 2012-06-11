#include "InputManager.h"
#include "InputClass.h"

InputManager::InputManager(void)
{
}


InputManager::~InputManager(void)
{
}

void InputManager::Initialize(InputClass* ic)
{
	m_inputClass = ic;
}

bool InputManager::IsKeyDown(unsigned int key)
{
	return m_inputClass->IsKeyDown(key);
}

std::string InputManager::GetKeyBoardState()
{
	std::string sstr = "Keyboard Press: ";
	if (IsKeyDown(DIK_UP))
	{
		sstr.append("UP ");
	}
	if (IsKeyDown(DIK_DOWN))
	{
		sstr.append("DOWN ");
	}
	if (IsKeyDown(DIK_LEFT))
	{
		sstr.append("LEFT ");
	}
	if (IsKeyDown(DIK_RIGHT))
	{
		sstr.append("RIGHT ");
	}
	if (IsKeyDown(DIK_Z))
	{
		sstr.append("Z ");
	}
	if (IsKeyDown(DIK_X))
	{
		sstr.append("X ");
	}
	if (IsKeyDown(DIK_LCONTROL))
	{
		sstr.append("CONTROL ");
	}
	return sstr;

}
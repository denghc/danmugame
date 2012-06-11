#pragma once
#include <string>
class InputClass;


class InputManager
{
public:
	InputManager(void);
	~InputManager(void);

public:
	void Initialize(InputClass* ic);

	bool IsKeyDown(unsigned int key);

	std::string GetKeyBoardState();
private:
	InputClass* m_inputClass;
};


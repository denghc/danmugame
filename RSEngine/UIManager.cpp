#include "UIManager.h"
#include "textclass.h"


UIManager::UIManager(void)
{
	
}


UIManager::~UIManager(void)
{
}


void UIManager::InitText()
{
	// frame count
	m_textClass->AddText("FrameCount", 720, 5, 1.0f, 1.0f, 1.0f);
	m_textClass->AddText("KeyBoard", 10, 5, 1.0f, 1.0f, 1.0f);
	m_textClass->AddText("HitCondition", 500, 5, 1.0f, 1.0f, 1.0f);
	m_textClass->AddText("SurviveTime", 10, 20, 1.0f, 0.0f, 0.0f);
	m_textClass->AddText("Score", 300, 20, 0.0f, 1.0f, 0.0f);

}

void UIManager::UpdateFrameCount(unsigned int fps, unsigned int frameCount)
{
	char cstr[200] = {0};
	sprintf(cstr, "FPS: %d", fps);
	std::string sstr;
	sstr.assign(cstr, strlen(cstr));
	m_textClass->EditText("FrameCount", sstr);
}

void UIManager::UpdateKeyboardInput(string keyBoardState)
{
	m_textClass->EditText("KeyBoard", keyBoardState);
}

void UIManager::UpdateScore(int score)
{
	char cstr[200] = {0};
	sprintf(cstr, "Score: %d", score);
	std::string sstr;
	sstr.assign(cstr, strlen(cstr));
	m_textClass->EditText("Score", sstr);
}

void UIManager::UpdateSurviveTime(float stime)
{
	char cstr[200] = {0};
	sprintf(cstr, "Survive Time: %.2fs", stime);
	std::string sstr;
	sstr.assign(cstr, strlen(cstr));
	m_textClass->EditText("SurviveTime", sstr);
}

void UIManager::Initialize(TextClass* tc)
{
	m_textClass = tc;
	InitText();
}

void UIManager::UpdateHitCondition(bool hit)
{
	char cstr[200] = {0};
	
	if (hit)
	{
		sprintf(cstr, "Hit!");
		
	}
	else
	{
		sprintf(cstr, "Alive!");
	}
	std::string sstr;
	sstr.assign(cstr, strlen(cstr));
	m_textClass->EditText("HitCondition", sstr);
}
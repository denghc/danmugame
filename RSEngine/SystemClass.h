#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#define RS_DEBUG

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "InputClass.h"
#include "GraphicsClass.h"
#include "audioClass.h"

class RSTimer;

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();
	static HWND GetWindowHandler()
	{
		return m_hwnd;
	};


	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	/*
		update by a frame
		just update data, not including the graphics
	*/
	bool Update();
	/*
		just render with the param interpolation
		the interpolation is the time in ms elapsed since last frame
	*/
	bool Render(float interpolation);
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	static HWND m_hwnd;
	InputClass* m_Input;
	GraphicsClass* m_Graphics;
	AudioClass* m_Sound;
	RSTimer* m_timer;
};

/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/////////////
// GLOBALS //
/////////////
static SystemClass* ApplicationHandle = 0;

#endif
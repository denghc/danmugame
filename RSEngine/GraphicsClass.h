////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

//////////////
// INCLUDES //
//////////////
#include "D3DClass.h"
#include "modelclass.h"
#include "shaderclass.h"
#include "spriteclass.h"
#include "GaussianMain.h"
#include "LightningMain.h"
#include "PerlinFire.h"

class CameraManager;
class CameraClass;
/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class TextClass;
class RenderObjectManager;
class TextureManager;
class ShaderManager;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	bool Render();

	static D3DClass* m_D3D;// TODO:coupling!!!
private:
	void InitializeResource(ID3D11Device*);

private:
	CameraClass* m_camera;
	CameraManager* m_cameraManager; 
	RenderObjectManager* m_renderObjMgr;
	TextureManager* m_textrueMgr;
	ShaderManager* m_shaderMgr;
	
	GaussianMain* m_gaussianMain;
	LightingMain* m_lightingMain;
	PerlinFire* m_perlinFire;
/*
	for communication with class GameLogic
*/
	// UI
private:
	TextClass* m_textClass;
public:
	TextClass* GetTextClass();

};

#endif
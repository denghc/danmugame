////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
#include "RenderObjectManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "TriangleTest.h"
#include "SpriteClass.h"
#include "SphereClass.h"
#include "textclass.h"
#include "GameLogic.h"
#include "BulletStormTest.h"
#include <DxErr.h>
#include "CameraManager.h"
#include "CameraClass.h"

#include "SpaceshipModel.h"
#include "RSObjPS.h"
#include "RSObjVS.h"
#include "ShaderManager.h"
#include "SystemClass.h"

D3DClass* GraphicsClass::m_D3D = 0;

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_cameraManager = 0;
	m_shaderMgr = 0;
	m_renderObjMgr = 0;
	m_textrueMgr = 0;
	m_gaussianMain = false;

	m_gaussianMain = new GaussianMain();
	m_lightingMain = new LightingMain();
	m_perlinFire = new PerlinFire();
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Create the Direct3D object.
	m_D3D = D3DClass::GetInstance();
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_camera = new CameraClass("mainCamera");
	CameraManager::getInstance()->insertCamera(m_camera);
	if(!m_camera)
	{
		return false;
	}
	GameLogic::GetInstance()->InitCamera(m_camera);

	// Set the initial position of the camera.
	m_camera->SetPosition(0.0f, 0.0f, -GameLogic::GetInstance()->GetCameraRadius());

	m_renderObjMgr = RenderObjectManager::GetInstance();
	if (!m_renderObjMgr)
	{
		return false;
	}

	m_textrueMgr = TextureManager::GetInstance();
	if (!m_textrueMgr)
	{
		return false;
	}

	m_shaderMgr = ShaderManager::GetInstance();
	if (!m_shaderMgr)
	{
		return false;
	}
	
	InitializeResource(m_D3D->GetDevice());
	//m_camera->SetRotation(30, 0, 0);

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the camera object.
	if(m_cameraManager)
	{
		m_cameraManager->Shutdown();
		delete m_cameraManager;
		m_cameraManager = 0;
	}

	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	if (m_textrueMgr)
	{
		m_textrueMgr->Shutdown();
		delete m_textrueMgr;
		m_textrueMgr = 0;
	}

	if (m_shaderMgr)
	{
		m_shaderMgr->Shutdown();
		delete m_shaderMgr;
		m_shaderMgr = 0;
	}

	if (m_renderObjMgr)
	{
		m_renderObjMgr->Shutdown();
		delete m_renderObjMgr;
		m_renderObjMgr = 0;
	}

	if(m_gaussianMain)
	{
		delete m_gaussianMain;
		m_gaussianMain = 0;
	}

	if(m_lightingMain)
	{
		delete m_lightingMain;
		m_lightingMain = 0;
	}

	if(m_perlinFire)
	{
		delete m_perlinFire;
		m_perlinFire = 0;
	}
	return;
}


bool GraphicsClass::Frame()
{
	bool result;



	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}
	return true;
}


bool GraphicsClass::Render()
{
	D3DXMATRIX viewMatrix, projectionMatrix, orthoMatrix;
	//bool result;
	static float xr = 0.0f;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.1f, 0.1f, 0.1f, 1.0f);
// 	m_camera->SetRotation(-xr / 3.1415926f * 180, 0, 0);
// 	m_camera->SetPosition(0,  -100.0f * sin(xr), -100.0f * cos(xr));
// 	xr += 0.0000001f;
// 	// Generate the view matrix based on the camera's position.
	m_camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_camera->GetViewMatrix(viewMatrix);
	//m_camera->SetRotation(0, 0, xr);
	xr += 0.1f;
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	int blfc = GameLogic::GetInstance()->NeedBlur();
	if (blfc > 0) 
	{
		m_gaussianMain->SetFR(180 - blfc);
		m_gaussianMain->OnD3D11FrameRender1(m_D3D->GetDeviceContext());
	}
	
	ID3D11DeviceContext *context = m_D3D->GetDeviceContext();
	ID3D11DepthStencilView*	m_scene_depth_stencil_view;
	ID3D11RenderTargetView*	m_scene_render_target_view;
	context->OMGetRenderTargets(1,&m_scene_render_target_view,&m_scene_depth_stencil_view);
	m_lightingMain->SetLightningRendererRTV_DSV(m_scene_depth_stencil_view,m_scene_render_target_view);
	m_lightingMain->OnD3D11FrameRender(viewMatrix, projectionMatrix);

	
	m_perlinFire->OnD3D11FrameRender(m_D3D->GetDeviceContext(), viewMatrix, projectionMatrix);

	//m_D3D->SetRasterState();
	m_renderObjMgr->Render(m_D3D->GetDeviceContext(), viewMatrix, projectionMatrix, orthoMatrix);
	if (blfc > 0)
		m_gaussianMain->OnD3D11FrameRender2(m_D3D->GetDeviceContext());

	m_D3D->EndScene();

	return true;
}

// temp function to load detailed resources into the 
void GraphicsClass::InitializeResource(ID3D11Device* device)
{
	// TestVS
	// TestPS
	// SquareTest
	RenderObjectManager* rom = RenderObjectManager::GetInstance();
	ShaderManager* shaderMgr = ShaderManager::GetInstance();
	/*
		Triangle Example
	*/
// 	TriangleTest* sqtest = new TriangleTest();
// 	sqtest->Initialize(device);
// 	rom->InsertRenderObject((RenderObject*) sqtest);


	/*
		bullet storm test
	*/
	BulletStormTest* bst = new BulletStormTest();
	bst->Initialize(device);
	rom->InsertRenderObject((RenderObject*)bst);

	/*
		Sphere Example
	*/

// 	SphereClass* omc = new SphereClass("sphere.obj");
// 	omc->Initialize(device);
// 	rom->InsertRenderObject((RenderObject*) omc);

	D3DXMATRIX baseViewMatrix;
	m_camera->Render();
	m_camera->GetViewMatrix(baseViewMatrix);// used by objects that are always normal against camera
	/*
	SpriteClass* sc = new SpriteClass();
	sc->Initialize(device);
	sc->LoadTexture(device, "dd.jpg");
	sc->SetAttributes(800, 600,  400, 600, baseViewMatrix);
	ID3D11DeviceContext* dc;
	device->GetImmediateContext(&dc);
	sc->UpdateAttributes(dc, 0, 0);
	rom->InsertRenderObject((RenderObject*)sc);
	*/
	/*
		text example
	*/
	m_textClass = new TextClass();

	
	
	m_textClass->SetAttributes(800, 600, baseViewMatrix);
	m_textClass->Initialize(device);
	GameLogic::GetInstance()->InitUIMgr(m_textClass);
	//rom->InsertRenderObject(m_textClass);

	//////////////////////////////////////////////////////////////////////////
	// This is the initialization of all resources
	//////////////////////////////////////////////////////////////////////////
	// Initialize Texture
	//////////////////////////////////////////////////////////////////////////

	// Insert Textures
	TextureManager::GetInstance()->CreateAndInsertTexture("Resource\\blt.png");


	// Initialize Shader
	//////////////////////////////////////////////////////////////////////////
	// rsobj shader ps vs
	
	RSObjPS* rsps = new RSObjPS("RSObjPS");
	rsps->Initialize(device, SystemClass::GetWindowHandler(), L"rsobjtest.hlsl", "PS");
	shaderMgr->InsertShader(SHADER_TYPE_PS, rsps);

	RSObjVS* rsvs = new RSObjVS("RSObjVS");
	rsvs->Initialize(device, SystemClass::GetWindowHandler(), L"rsobjtest.hlsl", "VS");
	shaderMgr->InsertShader(SHADER_TYPE_VS, rsvs);

	// Initialize RenderObject
	//////////////////////////////////////////////////////////////////////////
	SpaceshipModel* sm = new SpaceshipModel("tst.obj");
	sm->Initialize(device);
	rom->InsertRenderObject(sm);
	
	//////////////////////////////////////////////////////////////////////////
	// Effect Classes
	//////////////////////////////////////////////////////////////////////////

	m_gaussianMain->OnD3D11CreateDevice(device);
	m_lightingMain->OnD3D11CreateDevice(device);
	m_perlinFire->OnD3D11CreateDevice(device);

	//////////////////////////////////////////////////////////////////////////
	// Strange....
	//////////////////////////////////////////////////////////////////////////

	rom->InsertRenderObject((RenderObject*)bst);
	rom->InsertRenderObject(m_textClass);
}
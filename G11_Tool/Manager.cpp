//=============================================================================
// �}�l�[�W���[�N���X
// Createby Arai Yuhki
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include<process.h>

#include "Input\VC.h"
#include "Manager.h"
#include "Light.h"
#include "Camera.h"
#include "Camera2D.h"
#include "Renderer.h"
#include "Orbit2D.h"
#include "Sprite.h"
#include "Font.h"
#include "Shaim3DEffect.h"
#include "ShimmerParticle2D.h"
#include "LightParticle.h"
#include "Mutex.h"
#include "Loading.h"
//#include "Model.h"

#include "Scene.h"
#include "Title.h"
#include "Option.h"
#include "Game.h"
#include "Result.h"
//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
Renderer* Manager::_Render = nullptr;
CDebugProc* Manager::Debug = nullptr;
Manager* Manager::Self = nullptr;

#ifdef _DEBUG
bool ScreenShotMode = false;

bool GetScreenShotMode(void);
#endif

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Manager::Manager()
{
	_Render = nullptr;
}
//=============================================================================
// ������
//=============================================================================
void Manager::Initialize(void)
{
	Self = new Manager;
}

HRESULT Manager::Init(HINSTANCE hInstance,HWND hWnd,BOOL bWindow)
{
	_Render = new Renderer;
	if (_Render == nullptr)
	{
		return E_FAIL;
	}

	if (FAILED(_Render->Init(hWnd,bWindow)))
	{
		return E_FAIL;
	}
	if (FAILED(VC::Instance()->Init(hWnd)))
	{
		return E_FAIL;
	}

	Debug = new CDebugProc;
	Debug->Init();
	CLoading::Instance()->Init();

	CLoading::Instance()->Update();
	
	Scene = new Game;//�ŏ��̃V�[���𐶐�

	Sound::Initialize();
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,Thread,InitTexture,0,NULL);
	DWORD code = 0;
	while (true)
	{
		CLoading::Instance()->Update();
		GetExitCodeThread(handle,&code);
		if (code != STILL_ACTIVE)
		{
			CloseHandle(handle);
			break;
		}
	}
	handle = (HANDLE)_beginthreadex(NULL,0,SceneInit,this,0,NULL);
	while (true)
	{
		CLoading::Instance()->Update();
		GetExitCodeThread(handle,&code);
		if (code != STILL_ACTIVE)
		{
			CloseHandle(handle);
			break;
		}
	}
	
	Renderer::SetFade(60,Fade::FADE_IN,D3DXCOLOR(0,0,0,0));
	Current = Next = SCENE_GAME;
	SceneChangeFlag = false;
	
	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void Manager::Finalize(void)
{
	if (Self != nullptr)
	{
		Self->Uninit();
		delete Self;
		Self = nullptr;
	}
}
void Manager::Uninit(void)
{
	if (Debug != nullptr)
	{
		Debug->Uninit();
		delete Debug;
		Debug = nullptr;
	}
	if (Scene != nullptr)
	{
		Scene->Uninit();
		delete Scene;
		Scene = nullptr;
	}
	Sound::Finalize();
	ReleaseObject();
	VC::Instance()->Uninit();
	Camera3D::ReleaseAll();
	Camera2D::ReleaseAll();
	CLight::ReleaseAll();
	
	UninitTexture();
	
	CLoading::Finalize();
	Mutex::Finalize();
	if(_Render != nullptr)
	{
		_Render->Uninit();
		delete _Render;
		_Render = nullptr;
	}

#ifdef _DEBUG

#endif
}
//=============================================================================
// �X�V����
//=============================================================================
void Manager::Update(void)
{
	VC::Instance()->Update();
	Camera3D::UpdateAll();
	Camera2D::UpdateAll();
	Sound::Instance()->Update();
	if (Scene!=nullptr)
	{
		Scene->Update();
	}
	_Render->Update();


	ChangeScene();
	
	
}
//=============================================================================
// �`�揈��
//=============================================================================
void Manager::Draw(void)
{
	_Render->Draw();

}

void Manager::Control(void)
{

}

void Manager::ChangeScene(void)
{
	Fade* Fade = Fade::Instance();
	if (SceneChangeFlag && Fade->Active()==false)
	{
		if (Scene != nullptr)
		{
			Scene->Uninit();
			delete Scene;
			Scene = nullptr;
		}

		switch (Next)
		{
		case SCENE_TITLE:
			Scene = new Title;
			break;
		case SCENE_OPTION:
			Scene = new Option;
			break;
		case SCENE_GAME:
			Scene = new Game;
			break;
		case SCENE_RESULT:
			Scene = new Result;
			break;
		default:
			break;
		}

		if (Scene != nullptr)
		{
			HANDLE handle = (HANDLE)_beginthreadex(NULL,0,SceneInit,this,0,NULL);
			DWORD code = 0;
			while (true)
			{
				CLoading::Instance()->Update();
				GetExitCodeThread(handle,&code);
				if (code != STILL_ACTIVE)
				{
					CloseHandle(handle);
					break;
				}
			}
			Fade->Start(30,Fade::FADE_IN,BLACK(0.0f));
			SceneChangeFlag = false;
		}
	}
}

void Manager::SetScene(SCENE next)
{

	Next = next;
	Fade::Instance()->Start(30,Fade::FADE_OUT,BLACK(1.0f));
	SceneChangeFlag = true;

}
void Manager::ReleaseObject(void)
{
	String::ReleaseAll();
	Sprite::ReleaseAll();
	Shaim3DEffect::ReleaseAll();
	ShimmerParticle2D::ReleaseAll();
	LightParticle::ReleaseAll();
}

unsigned __stdcall Manager::Thread(void* func)
{
	Function function = (Function)func;
	function();

	_endthreadex(0);

	return 0;
}

unsigned __stdcall Manager::SceneInit(void* func)
{
	Manager* manager = (Manager*)func;
	manager->Scene->Init();
	_endthreadex(0);

	return 0;
}

void Manager::SetPause(bool flag)
{
	Sprite::SetPause(flag);
	Shaim3DEffect::SetPause(flag);
	Orbit2D::SetPause(flag);
	ShimmerParticle2D::SetPause(flag);
}
#ifdef _DEBUG
bool GetScreenShotMode(void)
{
	return ScreenShotMode;
}
#endif
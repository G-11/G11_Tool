#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//=============================================================================
//
// ���̒� [main.cpp]
// Author : Arai Yuhki
//
//=============================================================================
#include "main.h"
#include "Window.h"
#include "Manager.h"
#include "Input/Keyboard.h"
#include "Renderer.h"
#include<time.h>
#include "resource.h"

//#include "Mouse.h"

#ifdef _DEBUG
	#if CUSTOM_ALLOCAT_ENABLE
	#else
		#ifndef DBG_NEW
			#define DBG_NEW					new(_NORMAL_BLOCK,__FILE__,__LINE__)
			#define new DBG_NEW
		#endif
	#endif
#endif
//#endif
//=============================================================================
// �}�N����`
//=============================================================================


//=============================================================================
// �\���̒�`
//=============================================================================

//=============================================================================
// �O���[�o���ϐ�:
//=============================================================================


//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(72);
	
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	Window* window = Window::Instance();
	if (window->Initialize(hInstance,nCmdShow) == -1)
	{
		MessageBox(nullptr,"�f�o�C�X�������ł��܂���ł���","ERROR",MB_OK|MB_ICONERROR);
		return -1;
	}

	int msg = window->Run();

	window->Finalize();

	return msg;
	
}
#include "ScreenRender.h"

CScreenRender::CScreenRender()
{
	Effect = nullptr;
	ColorMode_ = 0;
}

CScreenRender::~CScreenRender()
{

	if (Effect != nullptr)
	{
		Effect->Release();
		Effect = nullptr;
	}
}

void CScreenRender::Init(void)
{
	LPD3DXBUFFER pError = NULL;

#ifdef _DEBUG
	if (FAILED(D3DXCreateEffectFromFile(Window::Instance()->Device(),"data/shaders/ScreenShade.fx",NULL,NULL,D3DXSHADER_SKIPVALIDATION,NULL,&Effect,&pError)))
	{
		MessageBox(nullptr,"�V�F�[�_�[�~�X","�x��",MB_OK);
		OutputDebugStringA((LPCSTR)pError->GetBufferPointer());
		pError->Release();
		pError = NULL;
		return;
	}
#else
	if (FAILED(D3DXCreateEffectFromFile(Window::Instance()->Device(),"data/shaders/ScreenShade.cso",NULL,NULL,D3DXSHADER_SKIPVALIDATION,NULL,&Effect,&pError)))
	{
		MessageBox(nullptr,"�V�F�[�_�[�~�X","�x��",MB_OK);
		OutputDebugStringA((LPCSTR)pError->GetBufferPointer());
		pError->Release();
		pError = NULL;
		return;
	}
#endif

	Projection2D = D3DXMATRIX(
		2.0f,0,0,0,
		0,-2.0f,0,0,
		0,0,1.0f,0,
		0,0,0,1.0f);
	Effect->SetMatrix("Projection2D",&Projection2D);

	DiffuseMap = Effect->GetParameterByName(NULL,"DiffuseMap");

	Effect->SetTechnique("Screen");
}

LPDIRECT3DTEXTURE9 CScreenRender::RenderScreen(LPDIRECT3DTEXTURE9 tex,int drawNum,bool clear)
{
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();

	if (clear){ Device->Clear(0L,nullptr,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,0x0,1.0f,0L); }
	
	Effect->SetTexture(DiffuseMap,tex);
	
	Effect->Begin(0,0);
	Effect->BeginPass(ColorMode_);
	//�`��
	if (drawNum < 2)
	{
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
	}
	else
	{
		for (int cnt = 0;cnt < drawNum;cnt++)
		{
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
		}
	}
	Effect->EndPass();

	Effect->End();
	

	return nullptr;
}

LPDIRECT3DTEXTURE9 CScreenRender::RenderWindow(LPDIRECT3DTEXTURE9 tex)
{
	LPDIRECT3DSURFACE9 OldSurface = nullptr;
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();

	Device->Clear(0L,nullptr,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,0x0,1.0f,0L);

	Effect->SetTexture(DiffuseMap,tex);

	Effect->Begin(0,0);
	Effect->BeginPass(0);
	//�`��
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

	Effect->EndPass();
	Effect->End();


	return nullptr;
}
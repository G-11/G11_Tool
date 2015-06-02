#include"Shimmer.h"
#include "ShimmerParticle2D.h"

CShimmer* CShimmer::Self = new CShimmer;

const char* CShimmer::ParamName[CShimmer::PARAM_MAX] = 
{
	"Fluctuation",
	"shimmerMap",
	"DiffuseMap",
};

CShimmer::CShimmer()
{
	Surface = nullptr;
	Blur[0] = nullptr;
	Blur[1] = nullptr;
	ResultSurface = nullptr;
	Texture = nullptr;
	BlurTex[0] = nullptr;
	BlurTex[1] = nullptr;
	Result = nullptr;
	Effect = nullptr;
	_BlurFlag = false;
}

CShimmer::~CShimmer()
{
	SafeRelease(Effect);
	if (Surface != nullptr)
	{
		Surface->Release();
		Surface = nullptr;
	}

	if (ResultSurface != nullptr)
	{
		ResultSurface->Release();
		ResultSurface = nullptr;
	}
	
	if (Result != nullptr)
	{
		Result->Release();
		Result = nullptr;
	}
	if (Texture != nullptr)
	{
		Texture->Release();
		Texture = nullptr;
	}

	for (int cnt = 0;cnt < 2;cnt++)
	{
		if (Blur[cnt] != nullptr)
		{
			Blur[cnt]->Release();
			Blur[cnt] = nullptr;
		}
		if (BlurTex[cnt] != nullptr)
		{
			BlurTex[cnt]->Release();
			BlurTex[cnt] = nullptr;
		}
	}
}
void CShimmer::Finalize(void)
{
	delete Self;
	Self = nullptr;
}

HRESULT CShimmer::Init(void)
{
	Window* window = Window::Instance();
	LPDIRECT3DDEVICE9 Device = window->Device();

	//�V�F�[�_�[���쐬
	LPD3DXBUFFER pError = NULL;
	if (FAILED(D3DXCreateEffectFromFile(Device,"data/shaders/ShimmerShader.fx",NULL,NULL,D3DXSHADER_SKIPVALIDATION,NULL,&Effect,&pError)))
	{
		MessageBox(nullptr,"�z���V�F�[�_�[�̐����Ɏ��s���܂���","ERROR",MB_OK | MB_ICONERROR);
		OutputDebugStringA((LPCSTR)pError->GetBufferPointer());
		pError->Release();
		pError = nullptr;
		return E_FAIL;
	}

	//�z���}�b�v�p�e�N�X�`�����쐬�A�T�[�t�F�C�X�ɃZ�b�g
	D3DXCreateTexture(Device,(UINT)window->WindowSize().x,(UINT)window->WindowSize().y,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&Texture);
	Texture->GetSurfaceLevel(0,&Surface);

	for (int cnt = 0;cnt < 2;cnt++)
	{
		D3DXCreateTexture(Device,(UINT)window->WindowSize().x,(UINT)window->WindowSize().y,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&BlurTex[cnt]);
		BlurTex[cnt]->GetSurfaceLevel(0,&Blur[cnt]);
	}

	//���ʗp�e�N�X�`�����쐬�A�T�[�t�F�C�X�ɃZ�b�g
	D3DXCreateTexture(Device,(UINT)window->WindowSize().x,(UINT)window->WindowSize().y,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&Result);
	Result->GetSurfaceLevel(0,&ResultSurface);

	D3DXMATRIX Projection = D3DXMATRIX(
		2.0f,0,0,0,
		0,-2.0f,0,0,
		0,0,1.0f,0,
		0,0,0,1.0f);
	Effect->SetMatrix("Projection",&Projection);
	D3DXVECTOR2 texel = D3DXVECTOR2(1.0f / window->WindowSize().x,1.0f / window->WindowSize().y);
	float U[5];
	float V[5];
	for (int cnt = 0;cnt < 5;cnt++)
	{
		U[cnt] = texel.x * (cnt + 1);
		V[cnt] = texel.y * (cnt + 1);
	}
	Effect->SetFloatArray("TexelU",U,5);
	Effect->SetFloatArray("TexelV",V,5);

	for (int cnt = 0;cnt < PARAM_MAX;cnt++)
	{
		Param[cnt] = Effect->GetParameterByName(NULL,ParamName[cnt]);
	}

	Effect->SetTechnique("Shimmer");

	return S_OK;
}

LPDIRECT3DTEXTURE9 CShimmer::Render(LPDIRECT3DTEXTURE9 tex)
{
	LPDIRECT3DSURFACE9 OldSurface = nullptr;
	LPDIRECT3DSURFACE9 OldDepth = nullptr;
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();

	//�[�x�o�b�t�@�͂��̂܂܎Q�Ƃ���
	Device->GetRenderTarget(0,&OldSurface);

	Device->SetRenderTarget(0,Surface);

	//�z���}�b�v���N���A����
	Device->Clear(0L,nullptr,(D3DCLEAR_TARGET | D3DCLEAR_STENCIL),0x80808080,1.0f,0);

	Effect->Begin(0,0);
	//�z���p�[�e�B�N����`��
	ShimmerParticle2D::DrawAll();

	//�w�i�𑗂�
	Effect->SetTexture(Param[BACK_BUFFER],tex);

	//�z���}�b�v�𑗂�
	Effect->SetTexture(Param[SHIMMER_MAP],Texture);

	if (_BlurFlag)
	{
		//�u���[��������
		ConductBlur();
	}
	else
	{
		Device->SetRenderTarget(0,ResultSurface);
	}
	Device->Clear(0L,nullptr,(D3DCLEAR_TARGET | D3DCLEAR_STENCIL),D3DXCOLOR(0.0f,0,0,0),1.0f,0);

	//�c�܂��ĕ`��
	
	Effect->BeginPass(0);

	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

	Effect->EndPass();
	Effect->End();

	//�ŏ��̃T�[�t�F�C�X�ɖ߂�
	Device->SetRenderTarget(0,OldSurface);

	//��n��
	if (OldSurface != nullptr)
	{
		OldSurface->Release();
		OldSurface = nullptr;
	}

	return Result;
}

void CShimmer::ConductBlur(void)
{
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();

	//�u���[X��������
	Device->SetRenderTarget(0,Blur[0]);
	Device->Clear(0L,nullptr,(D3DCLEAR_TARGET | D3DCLEAR_STENCIL),D3DXCOLOR(0.0f,0,0,0),1.0f,0);

	Effect->BeginPass(1);
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
	Effect->EndPass();

	//�u���[Y��������
	Device->SetRenderTarget(0,Blur[1]);
	Device->Clear(0L,nullptr,(D3DCLEAR_TARGET | D3DCLEAR_STENCIL),D3DXCOLOR(0.0f,0,0,0),1.0f,0);

	Effect->SetTexture(Param[SHIMMER_MAP],BlurTex[0]);

	Effect->BeginPass(2);
	Device->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
	Effect->EndPass();


	Device->SetRenderTarget(0,ResultSurface);
	Effect->SetTexture(Param[SHIMMER_MAP],BlurTex[1]);
}
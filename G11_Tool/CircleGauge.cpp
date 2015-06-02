//=============================================================================
//�~�`�Q�[�W�N���X
//=============================================================================
//=============================================================================
//�C���N���[�h
//=============================================================================
#include "CircleGauge.h"
#include "Shader2D.h"
#include "Renderer.h"
#include "Mutex.h"
//=============================================================================
//�R���X�g���N�^
//=============================================================================
CircleGauge::CircleGauge(int priority) :Sprite(priority)
{
	VtxBuff = nullptr;
	r =
	BaseAngle =
	Max = 
	DestCur =
	Current = 0;
	Divide =0;
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CircleGauge::~CircleGauge()
{
	SafeRelease(Point[0]);
	SafeRelease(Point[1]);
	SafeRelease(VtxBuff);
}
//=============================================================================
//����
//=============================================================================
CircleGauge* CircleGauge::Create(const D3DXVECTOR3& pos,float r,int divide,bool point,int priority)
{
	CircleGauge* Gauge = new CircleGauge(priority);
	if (Gauge == nullptr){ return nullptr; }

	Gauge->_Pos = pos;
	Gauge->r = r;
	Gauge->Divide = divide;
	Gauge->_Point = point;
	if (!Gauge->Init())
	{
		delete Gauge;
		return nullptr;
	}

	return Gauge;
}

//=============================================================================
//������
//=============================================================================
bool CircleGauge::Init(void)
{
	//���_�o�b�t�@���쐬
	Mutex::Instance()->Enter();
	if (FAILED(Window::Instance()->Device()->CreateVertexBuffer(sizeof(VERTEX_2D) * (Divide + 1),D3DUSAGE_WRITEONLY,FVF_CUSTOM,D3DPOOL_MANAGED,&VtxBuff,NULL)))
	{
		Mutex::Instance()->Leave();
		return false;
	}
	Mutex::Instance()->Leave();

	//�����Z�b�g
	VERTEX_2D* vtx;
	VtxBuff->Lock(0,0,(void**)&vtx,0);

	vtx[0].vtx = D3DXVECTOR3(0,0,0);
	vtx[0].tex = D3DXVECTOR2(0,1.0f);

	BaseAngle = (-360.0f)*(Current/Max);
	float angle = BaseAngle / (Divide-1);

	if (_Point)
	{
		Point[0] = Sprite::Create(D3DXVECTOR3(0,0,0),D3DXVECTOR2(70.0f,70.0f),WHITE(0.5f),Priority);
		Point[1] = Sprite::Create(D3DXVECTOR3(0,0,0),D3DXVECTOR2(70.0f,70.0f),WHITE(0.5f),Priority);

		Point[0]->SetTexture(GetTexture(TEX_GAGE_POINT));
		Point[1]->SetTexture(GetTexture(TEX_GAGE_POINT));

		Point[0]->SetPass(CShader2D::ADD);
		Point[1]->SetPass(CShader2D::ADD);
	}

	for (int cnt = 1;cnt < Divide + 1;cnt++)
	{
		vtx[cnt].vtx.x = sinf(DEG2RAD(angle*(cnt - 1) + 180.0f))*r;
		vtx[cnt].vtx.y = cosf(DEG2RAD(angle*(cnt - 1) + 180.0f))*r;
		vtx[cnt].vtx.z = 0;
		vtx[cnt].tex = D3DXVECTOR2(0,0);

		if (_Point)
		{
			if (cnt == 1)
			{
				Point[0]->SetPos(vtx[cnt].vtx);
			}
			else if (cnt == Divide)
			{
				Point[1]->SetPos(vtx[cnt].vtx);
			}
		}
	}

	VtxBuff->Unlock();

	Texture = GetTexture(TEX_GAGE);
	_Color = WHITE(1.0f);

	return true;
	
}
//=============================================================================
//�X�V
//=============================================================================
void CircleGauge::Update(void)
{
	float per = (Current / Max);//���݂̒l�͉���
	BaseAngle = (-360.0f)*per;
	BaseAngle /= (Divide-1);

	float sub = DestCur - Current;

	if (abs(sub / Max) < 0.005f)
	{
		Current += sub;
	}
	else
	{
		Current += sub*0.05f;
	}

	//���_�����Đݒ�
	VERTEX_2D* vtx;

	VtxBuff->Lock(0,0,(void**)&vtx,0);

	vtx[0].vtx = D3DXVECTOR3(0,0,0);

	D3DXVECTOR3 point = D3DXVECTOR3(0,0,0);

	for (int cnt = 1;cnt < Divide + 1;cnt++)
	{
		vtx[cnt].vtx.x = sinf(DEG2RAD(BaseAngle*(cnt - 1) + 180.0f))*r;
		vtx[cnt].vtx.y = cosf(DEG2RAD(BaseAngle*(cnt - 1) + 180.0f))*r;
		vtx[cnt].vtx.z = 0;
		if (cnt == 1)
		{
			point.x = sinf(DEG2RAD(BaseAngle*(cnt - 1) + 180.0f))*r*0.8f;
			point.y = cosf(DEG2RAD(BaseAngle*(cnt - 1) + 180.0f))*r*0.8f;
			if (_Point){ Point[0]->SetPos(point + _Pos); }
		}
		else if (cnt == Divide)
		{
			point.x = sinf(DEG2RAD(BaseAngle*(cnt - 1) + 180.0f))*r*0.8f;
			point.y = cosf(DEG2RAD(BaseAngle*(cnt - 1) + 180.0f))*r*0.8f;
			if (_Point){ Point[1]->SetPos(point + _Pos); }
		}
	}
	VtxBuff->Unlock();

}

//=============================================================================
//�`��
//=============================================================================
void CircleGauge::Draw(void)
{
	D3DXMATRIX MtxScl,MtxRot,MtxTrans;
	CShader2D* _Shader = CShader2D::Instance();

	WorldMtx = _Shader->Identity();


	D3DXMatrixScaling(&MtxScl,_Size.x,_Size.y,_Size.z);
	D3DXMatrixMultiply(&WorldMtx,&WorldMtx,&MtxScl);
	//��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&MtxRot,_Rot.y,_Rot.x,_Rot.z);
	D3DXMatrixMultiply(&WorldMtx,&WorldMtx,&MtxRot);


	//�ʒu�𔽉f
	D3DXMatrixTranslation(&MtxTrans,_Pos.x,_Pos.y,_Pos.z);
	D3DXMatrixMultiply(&WorldMtx,&WorldMtx,&MtxTrans);

	_Shader->SetMatrix(CShader2D::WORLD_MTX,WorldMtx);

	_Shader->SetFloatArray(CShader2D::DIFFUSE,_Color,4);
	_Shader->SetFloatArray(CShader2D::UV,uv,4);
	//�e�N�X�`���̐ݒ�
	_Shader->SetTexture(Texture);
	

	Window::Instance()->Device()->SetStreamSource(0,VtxBuff,0,sizeof(VERTEX_2D));
	Window::Instance()->SetVtxDecl(VTX_DECL_2D);
	//�|���S����`��
	
	_Shader->Draw(CShader2D::NORMAL,D3DPT_TRIANGLEFAN,Divide - 1);

	Renderer::SetStream2D();
	Window::Instance()->SetVtxDecl(VTX_DECL_2D);
}
//=============================================================================
//�C���N���[�h
//=============================================================================
#include "ShimmerParticle2D.h"
#include "Renderer.h"
#include "Shader2D.h"
//=============================================================================
//�O���[�o���ϐ�
//=============================================================================
int ShimmerParticle2D::_Num = 0;
ShimmerParticle2D* ShimmerParticle2D::Top_ = NULL;
ShimmerParticle2D* ShimmerParticle2D::Cur_ = NULL;
bool ShimmerParticle2D::PauseFlag = false;
CShader2D* ShimmerParticle2D::_Shader = nullptr;
//=============================================================================
//�R���X�g���N�^
//=============================================================================
ShimmerParticle2D::ShimmerParticle2D(void)
{

	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	//�o�b�t�@�̍쐬

	//�X�e�[�^�X�̏�����
	_Pos = D3DXVECTOR3(0,0,0);
	_Size = D3DXVECTOR3(1.0f,1.0f,1.0f);
	_Speed = D3DXVECTOR3(0,0,0);
	_Rot = D3DXVECTOR3(0,0,0);
	uv = D3DXVECTOR4(0,0,1.0f,1.0f);
	Texture = GetTexture(TEX_NONE);
	ReleaseFlag = false;
	frame = 0;
	_Num++;
	LinkList();
}
//=============================================================================
//���g�����X�g�ɒǉ�
//=============================================================================
void ShimmerParticle2D::LinkList(void)
{
	if (Top_ != NULL)//��ڈȍ~�̏���
	{
		ShimmerParticle2D* Polygon = Cur_;
		Polygon->Next_ = this;
		Prev_ = Polygon;
		Next_ = NULL;
		Cur_ = this;
	}
	else//�ŏ��̈�̎��̏���
	{
		Top_ = this;
		Cur_ = this;
		Prev_ = NULL;
		Next_ = NULL;
	}
}
//=============================================================================
//���g�����X�g����폜
//=============================================================================
void ShimmerParticle2D::UnlinkList(void)
{
	if (Prev_ == NULL)//�擪
	{
		if (Next_ != NULL)//��������
		{
			Next_->Prev_ = NULL;
			Top_ = Next_;
		}
		else//�Ō�̈������
		{
			Top_ = NULL;
			Cur_ = NULL;
		}
	}
	else if (Next_ == NULL)//�I�[
	{
		if (Prev_ != NULL)//�O������
		{
			Prev_->Next_ = NULL;
			Cur_ = Prev_;
		}
		else//�Ō�̈������
		{
			Top_ = NULL;
			Cur_ = NULL;
		}
	}
	else//�O��Ƀf�[�^������Ƃ�
	{
		Prev_->Next_ = Next_;
		Next_->Prev_ = Prev_;
	}

	Prev_ = NULL;
	Next_ = NULL;
	_Num--;
}
//=============================================================================
//���E�֐�
//=============================================================================
void ShimmerParticle2D::Release(void)
{
	UnlinkList();
	_Pos = D3DXVECTOR3(0,0,0);
	_Size = D3DXVECTOR3(0,0,0);
	_Speed = D3DXVECTOR3(0,0,0);
	_Rot = D3DXVECTOR3(0,0,0);
	frame = 0;

	delete this;

}
//=============================================================================
//�S����
//=============================================================================
void ShimmerParticle2D::ReleaseAll(void)
{
	ShimmerParticle2D* Polygon = Top_;
	ShimmerParticle2D* PolygonNext;
	while (Polygon)
	{
		PolygonNext = Polygon->Next_;
		Polygon->Release();
		Polygon = PolygonNext;
	}
	ShimmerParticle2D::Clear();

}
//=============================================================================
//�擪�ƏI�[��NULL��
//=============================================================================
void ShimmerParticle2D::Clear(void)
{
	Top_ = nullptr;
	Cur_ = nullptr;
}

//=============================================================================
//�|���S�����쐬
//=============================================================================
ShimmerParticle2D* ShimmerParticle2D::Create(const D3DXVECTOR3 &pos,const D3DXVECTOR2 &size,const D3DXCOLOR &color)
{
	ShimmerParticle2D* Polygon = new ShimmerParticle2D;
	Polygon->_Pos = pos;
	Polygon->_Size = D3DXVECTOR3(size.x,size.y,0);
	Polygon->_Color = color;

	return Polygon;
}
//=============================================================================
//�X�V
//=============================================================================
void ShimmerParticle2D::Update(void)
{
	if (ReleaseFlag)
	{
		Release();
		return;
	}

	_Pos += _Speed;

}
//=============================================================================
//�|�[�Y����
//=============================================================================
void ShimmerParticle2D::Pause(void)
{
	if (ReleaseFlag)
	{
		Release();
		return;
	}

}
//=============================================================================
//�S�̍X�V
//=============================================================================
void ShimmerParticle2D::UpdateAll(void)
{
	ShimmerParticle2D* Polygon = Top_;
	ShimmerParticle2D* Next = NULL;
	while (Polygon)
	{
		Next = Polygon->Next_;
		if (PauseFlag)
		{
			Polygon->Pause();
		}
		else
		{
			Polygon->Update();
		}

		Polygon = Next;
	}
}
//=============================================================================
//�|���S����`��
//=============================================================================
void ShimmerParticle2D::Draw(void)
{
	if (_Color.a > 0.01f)
	{
		D3DXMATRIX MtxScl,MtxRot,MtxTrans;
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
		/*
		_Shader->SetFloatArray(CShader2D::SIZE,_Size,3);
		_Shader->SetMatrix(CShader2D::ROT_MTX,MtxRot);
		_Shader->SetMatrix(CShader2D::POS_MTX,MtxTrans);
		*/

		_Shader->SetFloatArray(CShader2D::DIFFUSE,_Color,4);
		_Shader->SetFloatArray(CShader2D::UV,uv,4);
		//�e�N�X�`���̐ݒ�
		_Shader->SetTexture(Texture);

		//�|���S����`��
		_Shader->Draw(CShader2D::ADD,D3DPT_TRIANGLESTRIP);
	}
}
//=============================================================================
//���݂��Ă�|���S�������ׂĕ`��
//=============================================================================
void ShimmerParticle2D::DrawAll(void)
{
	if (_Shader == nullptr){ _Shader = CShader2D::Instance(); }

	ShimmerParticle2D* Polygon = Top_;

	_Shader->DrawBegin();
	while (Polygon)
	{
		Polygon->Draw();
		Polygon = Polygon->Next_;
	}

	_Shader->DrawEnd();
}

void ShimmerParticle2D::SetSpeed(const float base,float radian)
{
	_Speed.x = Sin_Rad(-radian)*base;
	_Speed.y = Cos_Rad(-radian)*base;
}

//=============================================================================
//UV�l�̐ݒ�
//	UNum	�����牽�Ԗڂ��H
//	VNum	�ォ�牽�Ԗڂ��H
//	DivideUNum	���̕�����(�Œ�1)
//	DivideVNum	�c�̕�����(�Œ�1);
//=============================================================================
void ShimmerParticle2D::SetUV(int UNum,int VNum,int DivideUNum,int DivideVNum)
{
	//�[���f�B�o�C�h�h�~
	if (DivideUNum == 0)
	{
		DivideUNum = 1;
	}
	if (DivideVNum == 0)
	{
		DivideVNum = 1;
	}
	float DivideU = 1.0f / DivideUNum;
	float DivideV = 1.0f / DivideVNum;

	uv.x = DivideU*UNum;
	uv.y = DivideV*VNum;
	uv.z = DivideU;
	uv.w = DivideV;
}

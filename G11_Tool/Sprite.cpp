//=============================================================================
//�C���N���[�h
//=============================================================================
#include "Sprite.h"
#include "Renderer.h"
#include "Camera2D.h"
#include "Shader2D.h"
//=============================================================================
//�O���[�o���ϐ�
//=============================================================================
int Sprite::PolygonNum=0;
Sprite* Sprite::Top_[LAYER_MAX] = {NULL};
Sprite* Sprite::Cur_[LAYER_MAX] = {NULL};
bool Sprite::PauseFlag = false;
CShader2D* Sprite::_Shader = nullptr;

const D3DXVECTOR3 Sprite::QuadBase[4] = {
	D3DXVECTOR3(-0.5f,-0.5f,0),
	D3DXVECTOR3(0.5f,-0.5f,0),
	D3DXVECTOR3(0.5f,0.5f,0),
	D3DXVECTOR3(-0.5f,0.5f,0),
};
//=============================================================================
//�R���X�g���N�^
//=============================================================================
Sprite::Sprite(int priority)
{

	//�X�e�[�^�X�̏�����
	_Color =
	_MaskColor = WHITE(1.0f);
	_Pos	= D3DXVECTOR3(0,0,0);
	_Offset = D3DXVECTOR2(0,0);
	_Size	= D3DXVECTOR3(1.0f,1.0f,1.0f);
	_Speed	= D3DXVECTOR3(0,0,0);
	_Rot		= D3DXVECTOR3(0,0,0);
	uv		= 
	MaskUV	= D3DXVECTOR4(0,0,1.0f,1.0f);
	Texture = 
	Mask = GetTexture(TEX_NONE);
	ReleaseFlag = false;
	Priority = priority;
	_Pass = CShader2D::NORMAL;
	memcpy(_Quad,QuadBase,sizeof(D3DXVECTOR3)*4);
	PolygonNum++;
	LinkList();
}
//=============================================================================
//���g�����X�g�ɒǉ�
//=============================================================================
void Sprite::LinkList(void)
{
	if(Top_[Priority] != NULL)//��ڈȍ~�̏���
	{
		Sprite* Polygon = Cur_[Priority];
		Polygon->Next_ = this;
		Prev_ = Polygon;
		Next_ = NULL;
		Cur_[Priority] = this;
	}
	else//�ŏ��̈�̎��̏���
	{
		Top_[Priority] = this;
		Cur_[Priority] = this;
		Prev_ = NULL;
		Next_ = NULL;
	}
}
//=============================================================================
//���g�����X�g����폜
//=============================================================================
void Sprite::UnlinkList(void)
{
	if(Prev_ == NULL)//�擪
	{
		if(Next_ != NULL)//��������
		{
			Next_->Prev_ = NULL;
			Top_[Priority] = Next_;
		}
		else//�Ō�̈������
		{
			Top_[Priority] = NULL;
			Cur_[Priority] = NULL;
		}
	}
	else if(Next_ == NULL)//�I�[
	{
		if(Prev_ != NULL)//�O������
		{
			Prev_->Next_ = NULL;
			Cur_[Priority] = Prev_;
		}
		else//�Ō�̈������
		{
			Top_[Priority] = NULL;
			Cur_[Priority] = NULL;
		}
	}
	else//�O��Ƀf�[�^������Ƃ�
	{
		Prev_->Next_ = Next_;
		Next_->Prev_ = Prev_;
	}

	Prev_ = NULL;
	Next_ = NULL;
	PolygonNum--;
}
//=============================================================================
//���E�֐�
//=============================================================================
void Sprite::Release(void)
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
void Sprite::ReleaseAll(void)
{
	for (int cnt = 0;cnt < LAYER_MAX;cnt++)
	{
		Sprite* Polygon = Top_[cnt];
		Sprite* PolygonNext;
		while (Polygon)
		{
			PolygonNext = Polygon->Next_;
			Polygon->Release();
			Polygon = PolygonNext;
		}
	}
	Sprite::Clear();
	
}
//=============================================================================
//�擪�ƏI�[��NULL��
//=============================================================================
void Sprite::Clear(void)
{
	Top_[0] = nullptr;
	Top_[1] = nullptr;
	Cur_[0] = nullptr;
	Cur_[1] = nullptr;
}

//=============================================================================
//�|���S�����쐬
//=============================================================================
Sprite* Sprite::Create(const D3DXVECTOR3 &pos,const D3DXVECTOR2 &size,const D3DXCOLOR &color,int priority)
{
	Sprite* Polygon = new Sprite(priority);
	Polygon->_Pos = pos;
	Polygon->_Size = D3DXVECTOR3(size.x,size.y,0);
	Polygon->_Color = color;

	return Polygon;
}
//=============================================================================
//�X�V
//=============================================================================
void Sprite::Update(void)
{
	
	_Pos += _Speed;

}
//=============================================================================
//�|�[�Y����
//=============================================================================
void Sprite::Pause(void)
{


}
//=============================================================================
//�S�̍X�V
//=============================================================================
void Sprite::UpdateAll(void)
{
	for (int cnt = 0;cnt < LAYER_MAX;cnt++)
	{
		Sprite* Polygon = Top_[cnt];
		Sprite* Next = NULL;
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
			if (Polygon->ReleaseFlag == true)
			{
				Polygon->Release();
			}

			Polygon = Next;
		}
	}
}
//=============================================================================
//�|���S����`��
//=============================================================================
void Sprite::Draw(void)
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

		CastMatrix();
		_Shader->SetFloatArray(CShader2D::OFFSET,_Offset,2);
		_Shader->SetFloatArray(CShader2D::DIFFUSE,_Color,4);
		_Shader->SetFloatArray(CShader2D::MASK_COLOR,_MaskColor,4);
		_Shader->SetFloatArray(CShader2D::UV,uv,4);
		_Shader->SetFloatArray(CShader2D::MASK_UV,MaskUV,4);
		//�e�N�X�`���̐ݒ�
		_Shader->SetTexture(Texture);
		_Shader->SetMask(Mask);

		//�|���S����`��
		_Shader->Draw((CShader2D::PASS)_Pass,D3DPT_TRIANGLESTRIP);
	}
}
//=============================================================================
//���݂��Ă�|���S�������ׂĕ`��
//=============================================================================
void Sprite::DrawAll(int priority)
{
	Renderer::SetStream2D();
	Sprite* Polygon = Top_[priority];
	if (_Shader == nullptr){ _Shader = CShader2D::Instance(); }
	
	if (priority == LAYER_INTERFACE || priority == LAYER_BACKGROUND){ Camera2D::GetCamera(0)->SetNoMove(); }
	else { Camera2D::Set(0); }
	
	_Shader->DrawBegin();
	while (Polygon)
	{
		Polygon->Draw();
		Polygon = Polygon->Next_;
	}
	_Shader->DrawEnd();
	
}
//=============================================================================
//����̃|���S�������� �������܂�g��Ȃ�
//=============================================================================
void Sprite::Delete(int index)
{
	if(index>-1&&index<PolygonNum)
	{
		Sprite* Polygon = Top_[0];
		for(int cnt=0;cnt<index;cnt++)
		{
			Polygon = Polygon->Next_;
		}
		if(Polygon != NULL)
		{
			Polygon->Release();
		}
	}
}

void Sprite::SetSpeed(const float base,float radian)
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
void Sprite::SetUV(int UNum,int VNum,int DivideUNum,int DivideVNum)
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
//=============================================================================
//UV�l�̐ݒ�
//	UNum	�����牽�Ԗڂ��H
//	VNum	�ォ�牽�Ԗڂ��H
//	DivideUNum	���̕�����(�Œ�1)
//	DivideVNum	�c�̕�����(�Œ�1);
//=============================================================================
void Sprite::SetMaskUV(int UNum,int VNum,int DivideUNum,int DivideVNum)
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

	MaskUV.x = DivideU*UNum;
	MaskUV.y = DivideV*VNum;
	MaskUV.z = DivideU;
	MaskUV.w = DivideV;
}

void Sprite::CastMatrix(void)
{

	for (int cnt = 0;cnt < 4;cnt++)
	{
		D3DXVECTOR3 quad = D3DXVECTOR3(QuadBase[cnt].x + _Offset.x,QuadBase[cnt].y + _Offset.y,0);
		D3DXVec3TransformCoord(&_Quad[cnt],&quad,&WorldMtx);
	}
}
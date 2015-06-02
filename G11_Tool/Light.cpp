#include "Light.h"

CLight* CLight::Top=NULL;
CLight* CLight::Cur=NULL;
int CLight::LightNum = 0;
//=============================================================================
//���g�����X�g�ɒǉ�
//=============================================================================
void CLight::LinkList(void)
{
	if(Top != NULL)//��ڈȍ~�̏���
	{
		CLight* pLight = Cur;
		pLight->Next = this;
		Prev = pLight;
		Next = NULL;
		Cur = this;
	}
	else//�ŏ��̈�̎��̏���
	{
		Top = this;
		Cur = this;
		Prev = NULL;
		Next = NULL;
	}
}
//=============================================================================
//���g�����X�g����폜
//=============================================================================
void CLight::UnlinkList(void)
{
	if(Prev == NULL)//�擪
	{
		if(Next != NULL)//��������
		{
			Next->Prev = NULL;
			Top = Next;
		}
		else//�Ō�̈������
		{
			Top = NULL;
			Cur = NULL;
		}
	}
	else if(Next == NULL)//�I�[
	{
		if(Prev != NULL)//�O������
		{
			Prev->Next = NULL;
			Cur = Prev;
		}
		else//�Ō�̈������
		{
			Top = NULL;
			Cur = NULL;
		}
	}
	else//�O��Ƀf�[�^������Ƃ�
	{
		Prev->Next = Next;
		Next->Prev = Prev;
	}

	Prev = NULL;
	Next = NULL;
}
//=============================================================================
//���ׂč폜
//=============================================================================
void CLight::ReleaseAll(void)
{
	CLight* pLight = Top;
	CLight* Next = NULL;
	while(pLight)
	{
		Next = pLight->Next;
		delete pLight;
		
		pLight = Next;
	}
}
//=============================================================================
//���g���폜
//=============================================================================
void CLight::Release(int index)
{
	CLight* pLight = Top;
	CLight* Next = NULL;
	int num=0;
	while(pLight)
	{
		Next = pLight->Next;
		if(num == index)
		{
			AllOff();//��x�S�Ẵ��C�g������
			pLight->UnlinkList();
			pLight->Vector = D3DXVECTOR3(0,0,0);
			pLight->Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
			pLight->Reset();//�Đݒ肷��
			delete pLight;
			
			break;
		}
		num++;
		pLight = Next;
	}
	
}
//=============================================================================
//�w�������C�g���쐬
//����: Vec �����x�N�g��
//		Color �F
//Create�����͍쐬�������C�g�̃|�C���^���Ԃ��Ă���
//=============================================================================
CLight* CLight::CreateDirectional(D3DXVECTOR3 Vec,D3DXCOLOR color)
{
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	CLight* pLight = new CLight;

	ZeroMemory(&pLight->Light,sizeof(D3DLIGHT9));

	pLight->Light.Type = D3DLIGHT_DIRECTIONAL;
	pLight->Light.Diffuse = color;
	pLight->Vector = Vec;

	D3DXVec3Normalize((D3DXVECTOR3*)&pLight->Light.Direction,&pLight->Vector);
	Device->SetLight(LightNum,&pLight->Light);
	Device->LightEnable(LightNum,TRUE);
	LightNum++;

	return pLight;
}
//=============================================================================
//�|�C���g���C�g���쐬(�X�y�L�����ƃA���r�G���g���f�B�t���[�Y�Ɠ����ɂ���)
//����:	D3DXVECTOR3	Pos		�����ʒu
//		float		Range	�e���͈�
//		float		Attenuation	�����W��	(0.0f�`1.0f 1,0f�Ō����Ȃ�)
//		D3DXCOLOR	diffuse		 �F
//=============================================================================
CLight* CLight::CreatePoint(D3DXVECTOR3 Pos,float Range,float Attenuation,D3DXCOLOR diffuse)
{
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	CLight* pLight = new CLight;

	ZeroMemory(&pLight->Light,sizeof(D3DLIGHT9));

	pLight->Light.Type = D3DLIGHT_POINT;
	pLight->Light.Diffuse = diffuse;
	pLight->Light.Ambient = diffuse;
	pLight->Light.Specular = diffuse;
	pLight->Light.Position = Pos;
	pLight->Light.Range = Range;
	pLight->Light.Attenuation0 = Attenuation;//��팸���W��
	pLight->Light.Attenuation1 = 0.0f;		//���`�����W��	AttenuationN�͑g�ݍ��킹����ł��낢��ω�����炵��
	pLight->Light.Attenuation2 = 0.0f;		//���������W��

	D3DXVec3Normalize((D3DXVECTOR3*)&pLight->Light.Direction,&pLight->Vector);
	Device->SetLight(LightNum,&pLight->Light);
	Device->LightEnable(LightNum,TRUE);
	LightNum++;

	return pLight;
}
//=============================================================================
//�|�C���g���C�g���쐬(�X�y�L�����ƃA���r�G���g��ʌɐݒ肷��)
//����:	D3DXVECTOR3	Pos		�����ʒu
//		float		Range	�e���͈�
//		float		Attenuation	�����W��	(0.0f�`1.0f 1,0f�Ō����Ȃ�)
//		D3DXCOLOR	diffuse		 �g�U���ˌ��F
//		D3DXCOLOR	Specular	 ���ʔ��ˌ��F
//		D3DXCOLOR	Ambient		 �����F
//=============================================================================
CLight* CLight::CreatePoint(D3DXVECTOR3 Pos,float Range,float Attenuation,D3DXCOLOR diffuse,D3DXCOLOR Specular,D3DXCOLOR Ambient)
{
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	CLight* pLight = new CLight;

	ZeroMemory(&pLight->Light,sizeof(D3DLIGHT9));

	pLight->Light.Type = D3DLIGHT_POINT;
	pLight->Light.Diffuse = diffuse;
	pLight->Light.Ambient = Ambient;
	pLight->Light.Specular = Specular;
	pLight->Light.Position = Pos;
	pLight->Light.Range = Range;
	pLight->Light.Attenuation0 = Attenuation;
	pLight->Light.Attenuation1 = 0.0f;
	pLight->Light.Attenuation2 = 0.0f;

	D3DXVec3Normalize((D3DXVECTOR3*)&pLight->Light.Direction,&pLight->Vector);
	Device->SetLight(LightNum,&pLight->Light);
	Device->LightEnable(LightNum,TRUE);
	LightNum++;

	return pLight;
}
//=============================================================================
//�X�|�b�g���C�g���쐬(�X�y�L�����ƃA���r�G���g���f�B�t���[�Y�Ɠ����ɐݒ肷��)
//����:	D3DXVECTOR3	Pos		�����ʒu
//		float		Range	�e���͈�
//		float		Attenuation	�����W��	(0.0f�`1.0f 1,0f�Ō����Ȃ�)
//		float		Theta	�����̃R�[���̊p�x	(0.0f�`��)
//		float		Phi		�O���̃R�[���̊p�x	(0.0f�`��)
//		float		FallOff	��������O���ւ̌�����(1.0f�ȊO�͏������x�̊֌W�Ŕ񐄏�)
//		D3DXCOLOR	diffuse		 �g�U���ˌ��F
//		D3DXCOLOR	Specular	 ���ʔ��ˌ��F
//		D3DXCOLOR	Ambient		 �����F
//=============================================================================
CLight* CLight::CreateSpot(D3DXVECTOR3 Vec,D3DXVECTOR3 Pos,float Range,float Attenuation,float Theta,float Phi,float FallOff,D3DXCOLOR diffuse)
{
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	CLight* pLight = new CLight;

	ZeroMemory(&pLight->Light,sizeof(D3DLIGHT9));

	pLight->Vector = Vec;		//����
	D3DXVec3Normalize((D3DXVECTOR3*)&pLight->Light.Direction,&pLight->Vector);

	pLight->Light.Type = D3DLIGHT_SPOT;
	pLight->Light.Diffuse = diffuse;
	pLight->Light.Ambient = diffuse;
	pLight->Light.Specular = diffuse;
	pLight->Light.Range = Range;
	pLight->Light.Attenuation0 = Attenuation;
	pLight->Light.Attenuation1 = 0.0f;
	pLight->Light.Attenuation2 = 0.0f;
	pLight->Light.Position = Pos;

	pLight->Light.Theta = Theta;//�����̃R�[���̊p�x
	pLight->Light.Phi = Phi;	//�O���̃R�[���̊p�x
	pLight->Light.Falloff = FallOff;	//��������O���Ɍ������ۂ̌�����
	
	Device->SetLight(LightNum,&pLight->Light);
	Device->LightEnable(LightNum,TRUE);
	LightNum++;

	return pLight;

}
//=============================================================================
//�X�|�b�g���C�g���쐬(�X�y�L�����ƃA���r�G���g��ʌɐݒ肷��)
//����:	D3DXVECTOR3	Pos		�����ʒu
//		float		Range	�e���͈�
//		float		Attenuation	�����W��	(0.0f�`1.0f 1,0f�Ō����Ȃ�)
//		float		Theta	�����̃R�[���̊p�x	(0.0f�`��)
//		float		Phi		�O���̃R�[���̊p�x	(0.0f�`��)
//		float		FallOff	��������O���ւ̌�����(1.0f�ȊO�͏������x�̊֌W�Ŕ񐄏�)
//		D3DXCOLOR	diffuse		 �g�U���ˌ��F
//		D3DXCOLOR	Specular	 ���ʔ��ˌ��F
//		D3DXCOLOR	Ambient		 �����F
//=============================================================================
CLight* CLight::CreateSpot(D3DXVECTOR3 Vec,D3DXVECTOR3 Pos,float Range,float Attenuation,float Theta,float Phi,float FallOff,D3DXCOLOR diffuse,D3DXCOLOR Specular,D3DXCOLOR Ambient)
{
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	CLight* pLight = new CLight;

	ZeroMemory(&pLight->Light,sizeof(D3DLIGHT9));
	
	pLight->Light.Type = D3DLIGHT_SPOT;
	pLight->Light.Diffuse = diffuse;
	pLight->Light.Ambient = Ambient;
	pLight->Light.Specular = Specular;
	pLight->Light.Range = Range;
	pLight->Light.Attenuation0 = Attenuation;
	pLight->Light.Attenuation1 = 0.0f;
	pLight->Light.Attenuation2 = 0.0f;
	pLight->Light.Position = Pos;

	pLight->Light.Theta = Theta;//�����̃R�[���̊p�x
	pLight->Light.Phi = Phi;	//�O���̃R�[���̊p�x
	pLight->Light.Falloff = FallOff;	//��������O���Ɍ������ۂ̌�����
	pLight->Vector = Vec;		//����

	D3DXVec3Normalize((D3DXVECTOR3*)&pLight->Light.Direction,&pLight->Vector);
	Device->SetLight(LightNum,&pLight->Light);
	Device->LightEnable(LightNum,TRUE);
	LightNum++;

	return pLight;

}
//=============================================================================
//index�Ԗڂ̃��C�g�̃X�C�b�`�𑀍�
//=============================================================================
void CLight::Swicth(int index,bool flag)
{
	CLight* pLight = Top;
	CLight* Next = NULL;
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	int num=0;
	while(pLight)
	{
		Next = pLight->Next;
		if(num == index)
		{
			BOOL Flag = FALSE;
			if(flag==true)
			{
				Flag = TRUE;
			}
			else
			{
				Flag = FALSE;
			}
			Device->LightEnable(index,Flag);
		}
		pLight = Next;
		num++;
	}
}
//=============================================================================
//Index�Ԗڂ̃��C�g�̃C���X�^���X���擾
//=============================================================================
CLight* CLight::GetLight(int Index)
{
	CLight* pLight = Top;
	CLight* Next = NULL;
	int num=0;
	while(pLight)
	{
		Next = pLight->Next;
		if(num == Index)
		{
			return pLight;
		}
		num++;
		pLight = Next;
	}
	return NULL;
}
//=============================================================================
//�Đݒ�
//=============================================================================
void CLight::Reset(void)
{
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	CLight* pLight = Top;
	int num=0;
	while(pLight)
	{
		Device->SetLight(num,&pLight->Light);
		num++;
		pLight = pLight->Next;
	}
}
//=============================================================================
//�S���̃��C�g��؂�
//=============================================================================
void CLight::AllOff(void)
{
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	CLight* pLight = Top;
	int num=0;
	while(pLight)
	{
		Device->LightEnable(num,FALSE);
		num++;
		pLight = pLight->Next;
	}
}
//=============================================================================
//�����ʒu���擾
//�w�������C�g�̏ꍇ�͑S��0���A��
//=============================================================================
D3DXVECTOR3 CLight::GetPos(void)
{
	if(Light.Type != D3DLIGHT_DIRECTIONAL)
	{
		return Light.Position;
	}
	return D3DXVECTOR3(0,0,0);
}
//=============================================================================
//���̉e���͈͂̎擾
//�w�������C�g�̏ꍇ��-1���A��
//=============================================================================
float CLight::GetRange(void)
{
	if(Light.Type != D3DLIGHT_DIRECTIONAL)
	{
		return Light.Range;
	}
	return -1;
}
//=============================================================================
//���̉e���͈͂̐ݒ�
//=============================================================================
void CLight::SetRange(float range)
{
	Light.Range = range;
	Reset();
}
//=============================================================================
//�F���Z�b�g
//=============================================================================
void CLight::SetColor(D3DXCOLOR color)
{
	Light.Diffuse = color;
	Reset();

}
//=============================================================================
//�F�����Z
//=============================================================================
void CLight::AddColor(D3DXCOLOR color)
{
	Light.Diffuse.r += color.r;
	Light.Diffuse.g += color.g;
	Light.Diffuse.b += color.b;
	Light.Diffuse.a += color.a;
	Reset();

}
//=============================================================================
//�������Z�b�g
//=============================================================================
void CLight::SetVec(D3DXVECTOR3 Vec)
{
	Vector = Vec;
	D3DXVec3Normalize((D3DXVECTOR3*)&Light.Direction,&Vector);
	Reset();

}
//=============================================================================
//�����ʒu���Z�b�g
//=============================================================================
void CLight::SetPos(D3DXVECTOR3 Pos)
{
	Light.Position = Pos;
	Reset();
}
//=============================================================================
//�����ʒu�����Z
//=============================================================================
void CLight::AddPos(D3DXVECTOR3 Pos)
{
	Light.Position.x += Pos.x;
	Light.Position.y += Pos.y;
	Light.Position.z += Pos.z;
	Reset();
}
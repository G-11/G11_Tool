//=============================================================================
//�|�����C���G�t�F�N�g2D�N���X
//=============================================================================
//=============================================================================
//�C���N���[�h
//=============================================================================
#include "Orbit2D.h"
#include "Renderer.h"
#include "Shader2D.h"
#include "Texture.h"
#include "Mutex.h"
//=============================================================================
//�O���[�o���ϐ�
//=============================================================================
Orbit2D* Orbit2D::Top_ = nullptr;
Orbit2D* Orbit2D::Cur_ = nullptr;
int Orbit2D::Num_ = 0;
CShader2D* Orbit2D::_Shader = nullptr;
bool Orbit2D::_PauseFlag = false;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
Orbit2D::Orbit2D()
{
	Parent_Pos = nullptr;
	Pos = D3DXVECTOR3(0,0,0);
	VtxBuff = nullptr;
	DivideNum = 0;
	Width_ = 0;
	_Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	ReleaseFlag_ = false;
	frame = 0;
	Pass = CShader2D::ADD;

	LinkList();
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
Orbit2D::~Orbit2D()
{
	UnlinkList();
}

//=============================================================================
//���g�����X�g�ɒǉ�
//=============================================================================
void Orbit2D::LinkList(void)
{
	if (Top_ != NULL)//��ڈȍ~�̏���
	{
		Orbit2D* Polygon = Cur_;
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
	Num_++;
}

//=============================================================================
//���g�����X�g����폜
//=============================================================================
void Orbit2D::UnlinkList(void)
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
	Num_--;
}
//=============================================================================
//�쐬
//�����FD3DXVECTOR3 �����ʒu
//		int ������
//		float ��
//		D3DXVECTOR3* �e�̍��W�|�C���^
//=============================================================================
Orbit2D* Orbit2D::Create(const D3DXVECTOR3& pos,int divide,float width,D3DXVECTOR3* parent,D3DXVECTOR3* parent2)
{
	Orbit2D* orbit = new Orbit2D;
	if (orbit == nullptr)
	{
		return orbit;
	}
	orbit->DivideNum = divide;
	orbit->Width_ = width;
	if (parent != nullptr && parent2 != nullptr)
	{
		orbit->Parent_Pos2[0] = parent;
		orbit->Parent_Pos2[1] = parent2;
	}
	else if (parent != nullptr)
	{
		orbit->Parent_Pos = parent;
	}
	orbit->Texture = GetTexture(TEX_NONE);
	orbit->Init(pos);

	return orbit;
}
//=============================================================================
//������
//�����F�������W
//=============================================================================
void Orbit2D::Init(const D3DXVECTOR3& pos)
{
	LPDIRECT3DDEVICE9 device = Window::Instance()->Device();

	Mutex::Instance()->Enter();
	device->CreateVertexBuffer(sizeof(VERTEX_2D)*(DivideNum * 2),D3DUSAGE_WRITEONLY,FVF_CUSTOM,D3DPOOL_MANAGED,&VtxBuff,NULL);
	Mutex::Instance()->Leave();

	//�e������Ȃ珉�����W���e�̍��W
	(Parent_Pos != nullptr) ? Pos = *Parent_Pos : Pos = pos;
	CountDown = DivideNum;
	VERTEX_2D* vtx;

	VtxBuff->Lock(0,0,(void**)&vtx,0);

	for (int cnt = 0;cnt < DivideNum * 2;cnt++)
	{
		vtx[cnt].vtx.x = Pos.x;
		vtx[cnt].vtx.y = (cnt % 2 == 0) ? Pos.y + Width_ / 2.0f : Pos.y - Width_ / 2.0f;
		vtx[cnt].vtx.z = 0;

		vtx[cnt].tex.x = (1.0f / DivideNum)*(cnt/2);
		vtx[cnt].tex.y = 1.0f - (cnt%2);
	}

	VtxBuff->Unlock();
}
//=============================================================================
//�X�V
//=============================================================================
void Orbit2D::Update(void)
{
	float angle = 0;
	D3DXVECTOR2 dis = D3DXVECTOR2(0,0);

	if (!ReleaseFlag_)
	{
		if (Parent_Pos != nullptr)
		{
			dis.x = (Parent_Pos->x - Pos.x);
			dis.y = (Parent_Pos->y - Pos.y);

			angle = -Atan2(dis.x,dis.y);
			Pos = *Parent_Pos;
		}
		else if (Parent_Pos2[0] != nullptr && Parent_Pos2[1] != nullptr)
		{
			Pos = *Parent_Pos2[0];
		}
		else
		{
			D3DXVECTOR2 oldPos = Pos;
			Pos += _Speed;
			dis.x = (Pos.x - oldPos.x);
			dis.y = (Pos.y - oldPos.y);

			angle = -Atan2(dis.x,dis.y);
		}
	}

	//��ԍŌ�̍��W���L��
	D3DXVECTOR3 tailPos = D3DXVECTOR3(0,0,0);

	//����������Ȃ̂ŁA���_�o�b�t�@��������
	VERTEX_2D* vtx;
	VtxBuff->Lock(0,0,(void**)&vtx,0);

	for (int cnt = DivideNum * 2 - 1;cnt > 1;cnt--)
	{//��Ƌ�����������O�̍��W���ڂ�
		vtx[cnt].vtx = vtx[(cnt-2)].vtx;
	}
	tailPos = vtx[DivideNum*2-1].vtx;
	
	if (!ReleaseFlag_)
	{//���̓�_�̓R�s�[�o���Ȃ��̂Ōv�Z������
		if (Parent_Pos2[0] != nullptr && Parent_Pos2[1] != nullptr)
		{
			vtx[0].vtx.x = Parent_Pos2[0]->x;
			vtx[0].vtx.y = Parent_Pos2[0]->y;

			vtx[1].vtx.x = Parent_Pos2[1]->x;
			vtx[1].vtx.y = Parent_Pos2[1]->y;
		}
		else
		{
			vtx[0].vtx.x = cosf(angle + PI)*Width_ / 2.0f + Pos.x;
			vtx[0].vtx.y = sinf(angle + PI)*Width_ / 2.0f + Pos.y;

			vtx[1].vtx.x = cosf(angle)*Width_ / 2.0f + Pos.x;
			vtx[1].vtx.y = sinf(angle)*Width_ / 2.0f + Pos.y;
		}
	}

	VtxBuff->Unlock();
	
	//�J���t���O�������Ă���Ȃ�
	if (ReleaseFlag_)
	{
		CountDown--;
		if (CountDown < 0)
		{//���̍��W�ƍŌ�̍��W���قڈ�v������J������
			Release();
		}
		
	}
}
//=============================================================================
// �|�[�Y
//=============================================================================
void Orbit2D::Pause(void)
{

}
//=============================================================================
//�`��
//=============================================================================
void Orbit2D::Draw(void)
{
	Window::Instance()->Device()->SetStreamSource(0,VtxBuff,0,sizeof(VERTEX_2D));

	_Shader->SetFloatArray(CShader2D::DIFFUSE,_Color,4);
	_Shader->SetTexture(Texture);

	_Shader->Draw((CShader2D::PASS)Pass,D3DPT_TRIANGLESTRIP,(DivideNum - 1) * 2);
}
//=============================================================================
//�J��
//=============================================================================
void Orbit2D::Release(void)
{
	SafeRelease(VtxBuff);
	delete this;
}
//=============================================================================
//�S���X�V
//=============================================================================
void Orbit2D::UpdateAll(void)
{
	Orbit2D* orbit = Top_;
	Orbit2D* next = nullptr;

	while (orbit)
	{
		next = orbit->Next_;

		if (_PauseFlag)
		{
			orbit->Pause();
		}
		else
		{
			orbit->Update();
		}

		orbit = next;
	}
}
//=============================================================================
//�S���`��
//=============================================================================
void Orbit2D::DrawAll(void)
{
	
	D3DXVECTOR4 uv(0,0,1.0f,1.0f);

	if (_Shader == nullptr){ _Shader = CShader2D::Instance(); }

	_Shader->SetIdentity(CShader2D::WORLD_MTX);
	Window::Instance()->SetVtxDecl(VTX_DECL_2D);

	_Shader->DrawBegin();
	Orbit2D* orbit = Top_;
	while (orbit)
	{
		orbit->Draw();

		orbit = orbit->Next_;
	}
	_Shader->DrawEnd();

	Renderer::SetStream2D();

}
//=============================================================================
//�S���J��
//=============================================================================
void Orbit2D::ReleaseAll(void)
{
	Orbit2D* orbit = Top_;
	Orbit2D* next = nullptr;

	while (orbit)
	{
		next = orbit->Next_;

		orbit->Release();

		orbit = next;
	}
}
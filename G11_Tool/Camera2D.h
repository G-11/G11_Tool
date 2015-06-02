//=============================================================================
// �J�����N���X
//Creater:Arai Yuhki
//=============================================================================
#ifndef _CAMERA2D_H_
#define _CAMERA2D_H_
//======================================================
// �C���N���[�h
//======================================================
#include "main.h"

class CShader2D;
class Wall;
class Player;

//======================================================
//�J�����N���X
//======================================================
class Camera2D
{
protected:
	static Camera2D* Top;
	static Camera2D* Cur;
	Camera2D* Prev;
	Camera2D* Next;
	virtual void Update(void);
	virtual void Set(void);
	void LinkList(void);
	void UnlinkList(void);
	void CreateViewPortMtx(D3DXMATRIX* pViewPort,UINT width,UINT height);

	static CShader2D* _Shader;
	D3DXVECTOR3 Pos;		//�J�����̈ʒu
	D3DXVECTOR3 DestPos;	//�ړ���
	D3DXVECTOR3 Speed;			//���x
	D3DXVECTOR3 Rot;			//�p�x
	D3DXMATRIX Projection2D;		//�v���W�F�N�V�����}�g���N�X
	D3DVIEWPORT9 ViewPort;		//�r���[�|�[�g
	float ForcusDisXZ;			//XZ���W�̋���
	float ForcusDisXY;			//XY���W�̋���
	D3DXVECTOR3 ClickPos;
	static int CameraNum;		//���݂̃J������
	int frame;

	Wall* Cilling;
	Wall* Floor;

	static Player* player;
	D3DXVECTOR3 OldPos;

	float _TopLimit;
	float _BottomLimit;
	float _LeftLimit;
	float _RightLimit;


public:
	Camera2D(const D3DXVECTOR2 &LUPos = D3DXVECTOR2(0,0),D3DXVECTOR2 &size = D3DXVECTOR2(SCREEN_WIDTH,SCREEN_HEIGHT));//�R�X�g���N�^
	virtual ~Camera2D();//�f�X�g���N�^
	static Camera2D* Create(const D3DXVECTOR3 &PPos);//�쐬(���C���J�����p�A��������)
	static Camera2D* Create(const D3DXVECTOR3 &PPos,const D3DXVECTOR2 &LUPos,D3DXVECTOR2 &size);//�쐬(�r���[�|�[�g�������ɐݒ�)
	static void UpdateAll(void);//�S���X�V
	/*
	DirectXDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	for(int cnt=0;cnt<Camera3D::GetCameraNum();cnt++)
	{
	Camera3D::Set(cnt);
	�`��
	}
	�ŕ`����B
	*/
	static void Set(int Index);//Index�Ԗڂ̃J�������g�p
	static void ReleaseAll(void);//�S�����
	void Destroy(void);			//�폜
	static void Clear(void);	//Top��Cur���N���A
	
	void SetNoMove(void);

	static int GetCameraNum(void)			{ return CameraNum; }//�J���������擾
	D3DXVECTOR3 GetPosP(void)const			{ return Pos; }//�J�����ʒu���擾
	D3DXVECTOR3 GetRot(void)const			{ return Rot; }
	D3DXVECTOR3 GetSpeed(void)const			{ return Speed; }
	D3DXVECTOR2 GetViewPortPos(void)const	{ return D3DXVECTOR2((float)ViewPort.X,(float)ViewPort.Y); }
	D3DXVECTOR2 GetViewPortSize(void)const	{ return D3DXVECTOR2((float)ViewPort.Width,(float)ViewPort.Height); }
	D3DXMATRIX Projection(void){ return Projection2D; }

	void SetPosP(const D3DXVECTOR3 &pos){ Pos = pos; }
	void SetSpeed	(const D3DXVECTOR3 &speed)		{ Speed = speed; }
	void SetRot		(const D3DXVECTOR3 &rot)			{ Rot = rot; }
	void SetViewPortPos(const D3DXVECTOR2 &Pos)	{ ViewPort.X = (DWORD)Pos.x; ViewPort.Y = (DWORD)Pos.y; }
	void SetViewPortSize(const D3DXVECTOR2 &Size)	{ ViewPort.Width = (DWORD)Size.x;ViewPort.Height = (DWORD)Size.y; }
	static void SetShader(CShader2D* shader){_Shader = shader;}
	void AddPosP(const D3DXVECTOR3 &pos)			{ Pos += pos; }
	void AddRot	(const D3DXVECTOR3 &rot)			{ Rot += rot; }
	void AddViewPortPos(const D3DXVECTOR2 &Pos)	{ ViewPort.X += (DWORD)Pos.x; ViewPort.Y += (DWORD)Pos.y; }
	void AddViewPortSize(const D3DXVECTOR2 &Size)	{ ViewPort.Width += (DWORD)Size.x;ViewPort.Height += (DWORD)Size.y; }

	static Camera2D* GetCamera(int Index);				//Index�Ԗڂ̃J�����̃|�C���^���擾

	void SetPlayer(Player* p){ player = p; }
	void SetCilling(Wall* cilling);
	void SetFloor(Wall* floor);

	void SetTopLimit(float limit){ _TopLimit = limit; }
	void SetBottomLimit(float limit){ _BottomLimit = limit; }
	void SetLeftLimit(float limit){ _LeftLimit = limit; }
	void SetRightLimit(float limit){ _RightLimit = limit; }

	float TopLimit(){ return _TopLimit; }
	float BottomLimit(){ return _BottomLimit; }
	float LeftLimit(){ return _LeftLimit; }
	float RightLimit(){ return _RightLimit; }
};

#endif
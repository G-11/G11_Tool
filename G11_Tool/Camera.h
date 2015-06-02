//=============================================================================
// �J�����N���X
//Creater:Arai Yuhki
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_
//======================================================
// �C���N���[�h
//======================================================
#include "main.h"
//======================================================
//�J�����N���X
//======================================================
class Camera3D
{
protected:
	static Camera3D* Top;
	static Camera3D* Cur;
	Camera3D* Prev;
	Camera3D* Next;
	virtual void Update(void);
	void LinkList(void);
	void UnlinkList(void);
	void CreateViewPortMtx(D3DXMATRIX* pViewPort,const UINT width,const UINT height);

	D3DXVECTOR3 PosCameraP;		//�J�����̈ʒu
	D3DXVECTOR3 DestPosCameraP;	//�ړ���?
	D3DXVECTOR3 PosCameraR;		//�����_
	D3DXVECTOR3 VecCameraU;		//�@���x�N�g��
	D3DXVECTOR3 Speed;			//���x
	D3DXVECTOR3 Rot;			//�p�x
	D3DXMATRIX ViewMatrix;		//�r���[�}�g���N�X
	D3DXMATRIX Projection;		//�v���W�F�N�V�����}�g���N�X
	D3DXMATRIX Projection2D;		//�v���W�F�N�V�����}�g���N�X
	D3DVIEWPORT9 ViewPort;		//�r���[�|�[�g
	D3DXVECTOR2 _Size;			//�A�X�y�N�g��Ɏg�p
	float ForcusDisXZ;			//XZ���W�̋���
	float ForcusDisXY;			//XY���W�̋���
	D3DXVECTOR3 ClickPos;
	static int CameraNum;		//���݂̃J������
	int frame;

public:
	Camera3D(const D3DXVECTOR2 &LUPos=D3DXVECTOR2(0,0),D3DXVECTOR2 &size=D3DXVECTOR2(SCREEN_WIDTH,SCREEN_HEIGHT));//�R�X�g���N�^
	virtual ~Camera3D();//�f�X�g���N�^
	static Camera3D* Create(const D3DXVECTOR3 &PPos,const D3DXVECTOR3 &RPos);//�쐬(���C���J�����p�A��������)
	static Camera3D* Create(const D3DXVECTOR3 &PPos,const D3DXVECTOR3 &RPos,const D3DXVECTOR2 &LUPos,D3DXVECTOR2 &size);//�쐬(�r���[�|�[�g�������ɐݒ�)
	static void UpdateAll(void);//�S���X�V
	virtual void Set(void);
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

	static int GetCameraNum(void)			{return CameraNum;}//�J���������擾
	D3DXVECTOR3 GetPosP(void)const			{return PosCameraP;}//�J�����ʒu���擾
	D3DXVECTOR3 GetPosR(void)const			{return PosCameraR;}//�����_���擾
	D3DXVECTOR3 GetRot(void)const			{return Rot;}
	D3DXVECTOR2 GetViewPortPos(void)const	{return D3DXVECTOR2((float)ViewPort.X,(float)ViewPort.Y);}
	D3DXVECTOR2 GetViewPortSize(void)const	{return D3DXVECTOR2((float)ViewPort.Width,(float)ViewPort.Height);}
	D3DXMATRIX GetViewMatrix(void)const		{return ViewMatrix;}

	void SetPosP(D3DXVECTOR3 Pos)			{PosCameraP = Pos;}
	void SetPosR(D3DXVECTOR3 Pos)			{PosCameraR = Pos;}
	void SetSpeed(D3DXVECTOR3 speed)		{Speed = speed;}
	void SetRot(D3DXVECTOR3 rot)			{Rot = rot;}
	void SetViewPortPos(D3DXVECTOR2 Pos)	{ViewPort.X = (DWORD)Pos.x; ViewPort.Y = (DWORD)Pos.y;}
	void SetViewPortSize(D3DXVECTOR2 Size)	{ViewPort.Width = (DWORD)Size.x;ViewPort.Height = (DWORD)Size.y;}
	void SetSize(const D3DXVECTOR2& size) { _Size = size; }

	void AddPosP(D3DXVECTOR3 Pos)			{PosCameraP += Pos;}
	void AddPosR(D3DXVECTOR3 Pos)			{PosCameraR += Pos;}
	void AddRot(D3DXVECTOR3 rot)			{Rot += rot;}
	void AddViewPortPos(D3DXVECTOR2 Pos)	{ViewPort.X += (DWORD)Pos.x; ViewPort.Y += (DWORD)Pos.y;}
	void AddViewPortSize(D3DXVECTOR2 Size)	{ViewPort.Width += (DWORD)Size.x;ViewPort.Height += (DWORD)Size.y;}
	
	static Camera3D* GetCamera(int Index);				//Index�Ԗڂ̃J�����̃|�C���^���擾

};

#endif
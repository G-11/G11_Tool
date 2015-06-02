//===========================================================
//2D�|���S���̏���
//Creater:Arai Yuhki
//===========================================================
#ifndef _SHIMMER_PARTICLE_2D_H_
#define _SHIMMER_PARTICLE_2D_H_
//===========================================================
//�C���N���[�h
//===========================================================
#include "main.h"

class CShader2D;
//===========================================================
//2D�|���S���N���X
//===========================================================
class ShimmerParticle2D
{
protected:
	D3DXVECTOR3 _Pos;		//�|���S���̈ʒu
	D3DXVECTOR3 _Size;		//�|���S���̑傫��
	D3DXVECTOR3 _Speed;		//�|���S���̈ړ����x
	D3DXVECTOR3 _Rot;		//�|���S���̊p�x
	D3DXVECTOR4 uv;			//x=left,y=top,z=width,w=height�Ƃ��Ĉ���
	D3DXCOLOR _Color;
	D3DXMATRIX WorldMtx;
	int frame;			//�t���[���J�E���g
	bool ReleaseFlag;
	static int _Num;
	LPDIRECT3DTEXTURE9 Texture;
	static bool PauseFlag;
	static CShader2D* _Shader;

	void LinkList(void);		//���g�����X�g�ɒǉ�
	void UnlinkList(void);		//���g�����X�g����폜
	virtual void Update(void);
	virtual void Pause(void);
	virtual void Draw(void);
	int Priority;

public:

	ShimmerParticle2D();
	virtual ~ShimmerParticle2D(){}
	static ShimmerParticle2D* Create(const D3DXVECTOR3 &pos,const D3DXVECTOR2 &size,const D3DXCOLOR &color);

	static void UpdateAll(void);

	static void DrawAll(void);

	void Release(void);			//�������폜
	static void ReleaseAll(void);

	static void Clear(void);


	//Getter&Stter
	static ShimmerParticle2D* Top(void){ return ShimmerParticle2D::Top_; }
	static ShimmerParticle2D* Cur(void){ return ShimmerParticle2D::Cur_; }
	ShimmerParticle2D* Prev(void)const{ return Prev_; }
	ShimmerParticle2D* Next(void)const{ return Next_; }

	D3DXCOLOR Color(void)const{ return _Color; }
	D3DXVECTOR3 Pos(void)const{ return _Pos; }
	D3DXVECTOR3 Rot(void)const{ return _Rot; }
	D3DXVECTOR3 Speed(void)const{ return _Speed; }
	D3DXVECTOR3 Size(void)const{ return _Size; }
	static int Num(void){ return _Num; }

	static void SetTop(ShimmerParticle2D* top){ Top_ = top; }
	static void SetCur(ShimmerParticle2D* cur){ Cur_ = cur; }
	static void SetPause(bool flag){ PauseFlag = flag; }
	void SetPos(const D3DXVECTOR3 &pos){ _Pos = pos; }
	void SetRot(const D3DXVECTOR3 &rot){ _Rot = rot; }
	void SetRot(const float &rot){ _Rot.z = rot; }
	void SetColor(const D3DXCOLOR &color){ _Color = color; }
	void SetSize(const D3DXVECTOR3 &size){ _Size = size; }
	void SetSpeed(const float base,float radian);
	void SetTexture(LPDIRECT3DTEXTURE9 tex){ Texture = tex; }
	void SetUV(int UNum = 0,int VNum = 0,int DivideUNum = 1,int DivideVNum = 1);
	void SetNext(ShimmerParticle2D* next){ Next_ = next; }
	void SetPrev(ShimmerParticle2D* prev){ Prev_ = prev; }
	void SetRelease(void){ ReleaseFlag = true; }

	void AddPos(const D3DXVECTOR3 &pos){ _Pos += pos; }
	void AddRot(const D3DXVECTOR3 &rot){ _Rot += rot; }
	void AddRot(const float &rot){ _Rot.z += rot; }
	void AddSize(const D3DXVECTOR3 &size){ _Size += size; }
	void AddColor(const D3DXCOLOR &color){ _Color += color; }

	//---------------------------

private:
	ShimmerParticle2D* Prev_;
	ShimmerParticle2D* Next_;

	static ShimmerParticle2D* Top_;
	static ShimmerParticle2D* Cur_;




};


#endif
//=============================================================================
//�|�����C���G�t�F�N�g2D�N���X
//=============================================================================
#ifndef _ORBIT2D_H_
#define _ORBIT2D_H_

//=============================================================================
//�C���N���[�h
//=============================================================================
#include "main.h"

class CShader2D;
//�N���X��`
class Orbit2D
{
public:
	Orbit2D();
	virtual ~Orbit2D();

	void LinkList(void);
	void UnlinkList(void);

	static Orbit2D* Top(void){ return Top_; }
	static Orbit2D* Cur(void){ return Cur_; }

	Orbit2D* Next(void){return Next_;}
	Orbit2D* Prev(void){return Prev_;}

	static void SetShader(CShader2D* shader){_Shader = shader;}
	static Orbit2D* Create(const D3DXVECTOR3& pos,int divide,float width,D3DXVECTOR3* parent = nullptr,D3DXVECTOR3* parent2 = nullptr);
	static void UpdateAll(void);
	static void DrawAll(void);
	static void ReleaseAll(void);

	static int Num(void){ return Num_; }
	static void SetPause(bool flag){ _PauseFlag = flag; }

	void SetHeadPos(const D3DXVECTOR3& pos){Pos = pos;}
	void SetSpeed(const D3DXVECTOR3& speed){_Speed = speed;}
	void SetColor(const D3DXCOLOR& color){_Color = color;}
	void SetTexture(LPDIRECT3DTEXTURE9 tex){Texture = tex;}
	void SetRelease(bool flag = true){ReleaseFlag_ = flag;}
	void SetPass(short pass){ Pass = pass; }

	D3DXVECTOR3 HeadPos(void)const{return Pos;}
	D3DXVECTOR3 Speed(void)const{return _Speed;}
	D3DXCOLOR Color(void)const{return _Color;}
	bool ReleaseFlag(void)const{return ReleaseFlag_;}

protected:

	void Init(const D3DXVECTOR3& pos);
	virtual void Update(void);
	virtual void Pause(void);
	virtual void Draw(void);
	virtual void Release(void);
	
	static Orbit2D* Top_;
	static Orbit2D* Cur_;
	Orbit2D* Next_;
	Orbit2D* Prev_;

	static int Num_;
	static CShader2D* _Shader;
	static bool _PauseFlag;

	D3DXVECTOR3* Parent_Pos;
	D3DXVECTOR3* Parent_Pos2[2];
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 _Speed;
	D3DXCOLOR _Color;
	int DivideNum;
	float Width_;

	LPDIRECT3DVERTEXBUFFER9 VtxBuff;
	LPDIRECT3DTEXTURE9 Texture;

	bool ReleaseFlag_;
	int frame;
	int CountDown;
	short Pass;
};

#endif
//=============================================================================
//�X�R�A�N���X
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//=============================================================================
//�C���N���[�h
//=============================================================================
#include "main.h"
#include "Sprite.h"

//=============================================================================
//�O���錾
//=============================================================================
class Number;

//�N���X��`
class Counter
{
public:
	static Counter* Create(const D3DXVECTOR3& pos,const D3DXVECTOR2& size,int CountPlace,bool camma = true);
	void SetNum(int Num){TargetScore = Num;}
	void SetCurrentNum(int num){ TargetScore = CurrentScore = num; }
	void AddNum(int Num){TargetScore += Num;}

	void SetPos(const D3DXVECTOR3& pos){ _Pos = DestPos = pos; }
	void SetDestPos(const D3DXVECTOR3& pos,float frame){DestPos = pos;MoveFrame = frame;}
	void SetMoveFrame(float frame){MoveFrame = frame;}
	void SetSize(const D3DXVECTOR2& size);
	void SetColor(const D3DXCOLOR& color);

	void AddPos(const D3DXVECTOR3& pos){ _Pos += pos;DestPos = _Pos; }
	void AddDestPos(const D3DXVECTOR3& pos){DestPos += pos;}
	void AddSize(const D3DXVECTOR2& size);
	void AddColor(const D3DXCOLOR& color);

	D3DXVECTOR3 Pos(void){ return _Pos; }
	D3DXCOLOR Color(void){ return _Color; }
	int GetNum(void){ return CurrentScore; }
	void Update(void);
	void Release(void);

private:
	int CountPlace;//����
	int CurrentScore;//���ݒl
	int TargetScore;//���̒l��ڎw���đ�������
	int MaxScore;//�\���\�ȍő�l
	float MoveFrame;
	D3DXVECTOR3 _Pos;
	D3DXVECTOR3 DestPos;
	D3DXVECTOR2 Size;
	D3DXCOLOR _Color;
	Number* number[20];
};

#endif
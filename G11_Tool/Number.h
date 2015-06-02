//=============================================================================
//�����\���N���X
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//=============================================================================
//�C���N���[�h
//=============================================================================
#include "main.h"
#include "Sprite.h"

//�N���X��`
class Number:public Sprite
{
public:
	Number(int priority = 1):Sprite(priority){}
	~Number(){}
	static Number* Create(D3DXVECTOR3 pos,D3DXVECTOR2 size,int num = 0,int priority = Sprite::LAYER_0);
	void SetNum(int num);
	int Num(void){ return _Number; }
private:
	int _Number;
};
#endif
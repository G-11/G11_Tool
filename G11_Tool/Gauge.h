#pragma once
#ifndef _GAUGE_H_
#define _GAUGE_H_
#include "Sprite.h"

typedef enum
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	DIRECTION_MAX
}DIRECTION;

class Gauge :public Sprite
{
public:
	Gauge(int priority);

	//=====================================================
	// ����
	//
	// �N�_�ƂȂ�ʒu
	// �傫��
	// �ŏ��l
	// �ő�l
	// �����l
	// �L�т����
	// �v���C�I���e�B
	//=====================================================
	static Gauge* Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,float min,float max,float current,DIRECTION direction = UP,int priority = Sprite::LAYER_INTERFACE);
	virtual void Init(void);
	void Update(void);

	float Min(void)const { return _Min; }
	float Max(void)const { return _Max; }
	float Current(void)const { return _Current; }
	float MaxSize(void)const { return _MaxSize; }
	float Per(void)const { return (_Current-_Min)/(_Max-_Min); }
	float Delay(void)const { return _Delay; }

	void SetMin(float min){ _Min = min; }
	void SetMax(float max){ _Max = max; }
	void SetDelay(float delay){ _Delay = delay; }

	void SetCurrent(float current){
		if (current > _Max){
			DestCurrent = _Max;
		}
		else if (current < _Min){
			DestCurrent = _Min;
		}
		else{
			DestCurrent = current;
		}
	}

	void SetMaxSize(float size){ _MaxSize += size; }

	void AddMin(float min){ _Min += min; }
	void AddMax(float max){ _Max += max; }
	void AddDelay(float delay){ _Delay += delay; }

	void AddCurrent(float num){
		float current = _Current + num;
		if (current > _Max){
			DestCurrent = _Max;
		}
		else if (current < _Min){
			DestCurrent = _Min;
		}
		else{
			DestCurrent = current;
		}
	}

	void AddMaxSize(float size){ _MaxSize += size; }

protected:
	float _Min;
	float _Max;
	float _MaxSize;//
	float _Current;
	float DestCurrent;
	float _Delay;	//0�`1.0f 1�ɋ߂��قǑ������݂̒l�܂ŕϓ�����
	DIRECTION Direction;

};

#endif
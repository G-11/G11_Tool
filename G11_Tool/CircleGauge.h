//=============================================================================
//�~�`�Q�[�W�N���X
//=============================================================================
#ifndef _CIRCLE_GAGE_H_
#define _CIRCLE_GAGE_H_

//=============================================================================
//�C���N���[�h
//=============================================================================
#include "main.h"
#include "Sprite.h"

//�N���X��`
class CircleGauge :public Sprite
{
public:
	CircleGauge(int priority = Sprite::LAYER_INTERFACE);
	~CircleGauge();

	static CircleGauge* Create(const D3DXVECTOR3& pos,float r,int divide,bool point = true,int priority = Sprite::LAYER_INTERFACE);

	bool Init(void);
	void Update(void);
	void Draw(void);

	void SetMax(float max){ Max = max; }//�ő�l��ݒ�
	void SetCur(float cur){ DestCur = cur; }//���ݒl��ݒ�

private:

	int Divide;//������
	float r;//���a
	float Max;//�ő�l
	float Current;//���ݒl
	float DestCur;//���X�Ɍ��������邽�߂̖ڕW�l
	float BaseAngle;//�J�n�n�_�̊p�x
	bool _Point;
	Sprite* Point[2];
	LPDIRECT3DVERTEXBUFFER9 VtxBuff;//���_�o�b�t�@
};

#endif
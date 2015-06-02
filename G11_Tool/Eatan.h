//================================================================================
//	���[����N���X
//	Author:Tsukagoshi Toshiyuki
//================================================================================
#pragma once 

#ifndef _EATAN_H_
#define _EATAN_H_

#include "Sprite.h"

class Eatan :public Sprite
{
public:
	typedef enum
	{
		EATAN_STATE_STOP = 0,		//�������ĂȂ�
		EATAN_STATE_MOVE,		//�ړ�
		EATAN_STATE_EAT,		//�H�ׂ�
		EATAN_STATE_GLAD,		//���
		EATAN_STATE_ATTACK,	//�U��
		EATAN_STATE_REVERSE,	//�f��
		EATAN_STATE_MAX,
		EATAN_STATE_NON,
	}EATAN_STATE;

	static Eatan* Create(const D3DXVECTOR3 &pos, const D3DXVECTOR2 &size, const D3DXCOLOR &color, int priority = 0);

	Eatan(int priority = 0);
	~Eatan(){}

	virtual void Update();
	virtual void Draw();

	void SetState(EATAN_STATE State);	//�����ɃX�e�[�g��ύX����
	void SetNextState(EATAN_STATE State){ NextState = State; }	//���݂̃��[�V�������I��莟��X�e�[�g��ύX����
	void SetSwayFlag(bool Flag){ SwayFlag = Flag; }
	EATAN_STATE State(){ return _State; }

protected :
	EATAN_STATE NextState;
	EATAN_STATE _State;
	EATAN_STATE OldState;

	float AnimationCount;			//�A�j���[�V�����p�J�E���^�[
	short AnimationPartition[EATAN_STATE_MAX];	//�A�j���[�V����������
	float AnimationSpeed[EATAN_STATE_MAX];		//�A�j���[�V�����X�s�[�h
	int   MaxPartition;				//�A�j���[�V���������̍ő�l

	bool SwayFlag;					//true�ŏ㉺�ɗh���
	float SwayCount;				//�㉺�ɗh���p�J�E���^�[
};

#endif
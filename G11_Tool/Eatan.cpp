//================================================================================
//	���[����N���X
//	Author:Tsukagoshi Toshiyuki
//================================================================================
#include "Eatan.h"
#include "Shader2D.h"
#include "Texture.h"


//================================================================================
//	�萔
//================================================================================
//�A�j���[�V�����̕�����
#define EATAN_PARTITION_STOP		(1)			
#define EATAN_PARTITION_MOVE		(2)
#define EATAN_PARTITION_EAT			(6)
#define EATAN_PARTITION_GLAD		(6)
#define EATAN_PARTITION_ATTACK		(2)
#define EATAN_PARTITION_REVERSE		(3)

//�A�j���[�V�����̃X�s�[�h
#define EATAN_ANIMATION_SPEED_STOP		(0.03f)
#define EATAN_ANIMATION_SPEED_MOVE		(0.03f)
#define EATAN_ANIMATION_SPEED_EAT		(0.2f)
#define EATAN_ANIMATION_SPEED_GLAD		(0.03f)
#define EATAN_ANIMATION_SPEED_ATTACK	(0.2f)
#define EATAN_ANIMATION_SPEED_REVERSE	(0.02f)

//�㉺�ɗh���ړ���
//
#define EATAN_SWAY_HEIGHT		(0.05f)

//�㉺�ɗh���X�s�[�h
#define EATAN_SWAY_SPEED		(0.04f)

//================================================================================
//	�R���X�g���N�^
//================================================================================
Eatan::Eatan(int priority) :Sprite(priority)
{
	//�X�e�[�^�X�̏�����
	_State = EATAN_STATE_STOP;			//��~��ԂŐ���
	OldState = EATAN_STATE_STOP;
	NextState = EATAN_STATE_NON;
	MaxPartition = 0;
	SwayCount = 0;

	AnimationPartition[EATAN_STATE_STOP] = EATAN_PARTITION_STOP;
	AnimationPartition[EATAN_STATE_MOVE] = EATAN_PARTITION_MOVE;
	AnimationPartition[EATAN_STATE_EAT] = EATAN_PARTITION_EAT;
	AnimationPartition[EATAN_STATE_GLAD] = EATAN_PARTITION_GLAD;
	AnimationPartition[EATAN_STATE_ATTACK] = EATAN_PARTITION_ATTACK;
	AnimationPartition[EATAN_STATE_REVERSE] = EATAN_PARTITION_REVERSE;

	AnimationSpeed[EATAN_STATE_STOP] = EATAN_ANIMATION_SPEED_STOP;
	AnimationSpeed[EATAN_STATE_MOVE] = EATAN_ANIMATION_SPEED_MOVE;
	AnimationSpeed[EATAN_STATE_EAT] = EATAN_ANIMATION_SPEED_EAT;
	AnimationSpeed[EATAN_STATE_GLAD] = EATAN_ANIMATION_SPEED_GLAD;
	AnimationSpeed[EATAN_STATE_ATTACK] = EATAN_ANIMATION_SPEED_ATTACK;
	AnimationSpeed[EATAN_STATE_REVERSE] = EATAN_ANIMATION_SPEED_REVERSE;

	//�A�j���[�V���������̍ő�l
	for (int i = 0; i < EATAN_STATE_MAX; i++)
	{
		MaxPartition = max(MaxPartition, AnimationPartition[i]);
	}
}

//================================================================================
//	����
//================================================================================
Eatan* Eatan::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR2 &size, const D3DXCOLOR &color, int priority)
{
	Eatan* eatan = new Eatan(priority);
	eatan->_Pos = pos;
	eatan->_Size = D3DXVECTOR3(size.x, size.y, 0);
	eatan->_Color = color;

	eatan->SetTexture(GetTexture(TEX_EATAN));
	eatan->SetSwayFlag(false);

	return eatan;
}

//================================================================================
//	�X�V
//================================================================================
void Eatan::Update()
{
	//�㉺�ɂӂ�ӂ�ړ�
	if (SwayFlag)
	{
		SwayCount += EATAN_SWAY_SPEED;
		SetOffsetY( sinf(SwayCount)*EATAN_SWAY_HEIGHT);
	}

	//�O��Ə�Ԃ��ς���Ă�����A�j���[�V�����p�J�E���^�����Z�b�g
	if (OldState != _State)
	{
		AnimationCount = 0.0f;
	}
	//�ύX����Ă��Ȃ���΃J�E���^�̍X�V
	else
	{
		AnimationCount += AnimationSpeed[_State];

		if (AnimationCount > AnimationPartition[_State])
		{
			AnimationCount = 0.0f;
			//�J��Ԃ��Ȃ����[�V����
			//�H�ׂ�
			//�U��
			if (_State == EATAN_STATE_EAT ||
				_State == EATAN_STATE_ATTACK ||
				_State == EATAN_STATE_GLAD ||
				_State == EATAN_STATE_REVERSE)
			{
				if (SwayFlag)
				{
					_State = EATAN_STATE_MOVE;
				}
				else
				{
					_State = EATAN_STATE_STOP;
				}
			}

			//���̃X�e�[�g���ݒ肳��Ă����炻����ɕύX
			if (NextState != EATAN_STATE_NON)
			{
				_State = NextState;
				NextState = EATAN_STATE_NON;
			}

		}
	}

	//�ړ������̔���	
	if (_Speed.x >= 0)
	{
		//�e�N�X�`��UV�̕ύX
		SetUV(D3DXVECTOR4((1.0f / MaxPartition)*(int)AnimationCount, (1.0f / EATAN_STATE_MAX)*_State, 1.0f / (float)MaxPartition, 1.0f / (float)EATAN_STATE_MAX));
	}
	else
	{
		//�e�N�X�`��UV�̕ύX
		SetUV(D3DXVECTOR4((1.0f / MaxPartition)*((int)AnimationCount + 1), (1.0f / EATAN_STATE_MAX)*_State, 1.0f / (float)-MaxPartition, 1.0f / (float)EATAN_STATE_MAX));
	}

	//�X�e�[�g�̕ۑ�
	OldState = _State;

	Sprite::Update();
}

//================================================================================
//�@�`�揈��
//================================================================================
void Eatan::Draw()
{
	Sprite::Draw();
}

//================================================================================
//	�X�e�[�g�̐ݒ�
//================================================================================
void Eatan::SetState(EATAN_STATE State)
{
	//�ȉ��̏�Ԃ̂Ƃ��̓X�e�[�g�̕ύX���󂯕t���Ȃ�
	//�H�ׂ�
	//�U��
	//�f��
	if (_State != EATAN_STATE_EAT &&
		_State != EATAN_STATE_ATTACK &&
		_State != EATAN_STATE_REVERSE)
	{
		_State = State;
	}
}
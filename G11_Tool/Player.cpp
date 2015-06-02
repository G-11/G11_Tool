//================================================================================
//	�v���C���[�N���X
//	Author:Tsukagoshi Toshiyuki
//================================================================================

//================================================================================
//	include
//================================================================================
#include "Player.h"
#include "Shader2D.h"
#include "Texture.h"
#include "Input\VC.h"
#include "Item.h"
#include "Interface.h"
#include "Wall.h"
#include "Collision.h"
#include "Camera2D.h"
#include "GrowupEffect.h"
//================================================================================
//	�萔
//================================================================================
//�ړ��X�s�[�h
#define PLAYER_SPEED		(0.5f)

//�����̌�����
#define PLAYER_INERTIA		(0.95f)

//���˕����̊J��(DEG)		
#define PLAYER_REFRECT_RAND (30.0f)

//�M�~�b�N�ɓ����������̉����o�����X�s�[�h
#define PLAYER_REFRECT_SPEED (5.0f)


//================================================================================
//	�R���X�g���N�^
//================================================================================
Player::Player(int Priority) :Eatan(Priority)
{
	HitCheckFlag = true;
	ActionFlag = true;
}

//================================================================================
//	�f�X�g���N�^
//================================================================================
Player::~Player()
{
	//�J�����������ʒu�ɕύX
	Camera2D* camera = Camera2D::GetCamera(0);
	camera->SetPosP(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//================================================================================
//	�v���C���[����
//================================================================================
Player* Player::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR2 &size, const D3DXCOLOR &color, int priority)
{
	Player* player = new Player(priority);
	player->_Pos = pos;
	player->_Size = D3DXVECTOR3(size.x, size.y, 0);
	player->_Color = color;

	player->SetTexture(GetTexture(TEX_EATAN));
	player->SetSwayFlag(true);
	player->SetState(EATAN_STATE_MOVE);

	return player;
}

//================================================================================
//	�X�V����
//================================================================================
void Player::Update()
{
	VC* vc = VC::Instance();

	//����t���O
	if (ActionFlag)
	{
		//****************************************
		//	�ړ�
		//****************************************

		_Speed += D3DXVECTOR3(vc->LStickX(), vc->LStickY(), 0.0f) * PLAYER_SPEED;
		_Speed *= PLAYER_INERTIA;


		//****************************************
		//	�U��
		//****************************************
		if (vc->Trigger(COMMAND_B))
		{
			SetState(EATAN_STATE_ATTACK);
		}

		//****************************************
		//	�H�ׂ�
		//****************************************
		if (vc->Trigger(COMMAND_A))
		{
			SetState(EATAN_STATE_EAT);

		}

		//�H�ׂ锻��
		if (_State == EATAN_STATE_EAT)
		{
			
			//�����蔻��
			Item* item = Item::HitCheck(_Pos, _Size);
			if (item != nullptr)
			{
				float score = item->Score();
				Interface::AddScore(score);
				item->Action(&_Pos, AnimationPartition[EATAN_STATE_EAT], (float)AnimationSpeed[EATAN_STATE_EAT]);
					GrowupEffect::Creates(_Pos,&_Pos,250.0f,60,12);

					if (score > 0)
					{
						SetNextState(EATAN_STATE_GLAD);
					}
					else
					{
						SetNextState(EATAN_STATE_REVERSE);
					}
			}
			
		}
	}

	//�����蔻��t���O
	if (HitCheckFlag)
	{
		List<Wall>* wall = Wall::HitList();
		VALUE<Wall>* itr = wall->Begin();

		while (itr)
		{
			D3DXVECTOR3 reflectVec(0, 0, 0);
			D3DXVECTOR3 *quad = itr->Data->Quad();

			if (Collision::CircleQuad(_Pos, 5.0f, quad, 4, _Speed, &reflectVec))
			{
				itr->Data->HitAffect();
				D3DXVECTOR3 wallSpeed = itr->Data->Speed();
				//�ǂ̈ړ��x�N�g������v���C���[�����̐����݂̂����Z
				D3DXVECTOR3 playerwall = _Pos - itr->Data->Pos();
				D3DXVec3Normalize(&playerwall, &playerwall);
				float wallReflect = D3DXVec3Dot(&playerwall, &wallSpeed);
				playerwall *= (wallReflect+PLAYER_REFRECT_SPEED);

				//���˃x�N�g���������_���ω�������
				float randReflect = Randf(-PLAYER_REFRECT_RAND*0.5, PLAYER_REFRECT_RAND*0.5);

				D3DXVECTOR3 speed = _Speed;
				float angle = atan2(reflectVec.x, reflectVec.y) + DEG2RAD(randReflect);
				float sp = D3DXVec3Length(&(speed + playerwall));
				_Speed.x = sinf(angle)*sp;
				_Speed.y = cosf(angle)*sp;
				angle = RAD2DEG(angle);
				SetPos(OldPos);
			}

			itr = itr->_Next;
		}

	}
	OldPos = _Pos;

	Eatan::Update();
}

//================================================================================
//	�`�揈��
//================================================================================
void Player::Draw()
{

	Eatan::Draw();
}
//================================================================================
//	�S�[���I�u�W�F�N�g
//	Author: Toshiyuki Tsukagoshi
//================================================================================
#include "Goal.h"
#include "Texture.h"
#include "Collision.h"
#include "Game.h"
#include "Player.h"
#include "Manager.h"
#include "Fade.h"

//================================================================================
//	�R���X�g���N�^
//================================================================================
Goal::Goal(int priority) :Sprite(priority)
{

}

//================================================================================
//	����
//================================================================================
Goal* Goal::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR2 &size, const D3DXCOLOR &color, int priority)
{
	Goal* goal = new Goal(priority);
	goal->_Pos = pos;
	goal->_Size = D3DXVECTOR3(size.x, size.y, 0.0f);
	goal->_Color = color;

	//�e�N�X�`���ł�����e�N�X�`���쐬
	goal->SetTexture(GetTexture(TEX_GOAL));

	return goal;
}

//================================================================================
//	�X�V
//================================================================================
void Goal::Update()
{
	if (Collision::Quad(_Pos, _Quad, 4, player->Pos(), player->Quad(), 4))
	{
		if (Fade::Instance()->Mode() == Fade::FADE_NONE)
		{
			Manager::Instance()->SetScene(Manager::SCENE_RESULT);
		}
	}

	Sprite::Update();
}

//================================================================================
//	�`��
//================================================================================
void Goal::Draw()
{
	Sprite::Draw();
}


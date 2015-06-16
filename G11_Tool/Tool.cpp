#include "main.h"
#include "Tool.h"
#include "Sprite.h"
#include "Input\VC.h"
#include "Input\Mouse.h"
#include "Input\Keyboard.h"
#include "Wall.h"
#include "Collision.h"
#include "Debug.h"
#include "Camera2D.h"


Tool* Tool::SelfPointer = new Tool;


Tool::Tool()
{
	SelectedObj = nullptr;
}

void Tool::Update()
{
	Mouse *mouse = VC::Instance()->mouse();
	Keyboard *key = VC::Instance()->keyboard();
	
	//�N���b�N���ꂽ��
	if (mouse->Trigger(Mouse::MOUSE_LEFT))
	{
		//�I�u�W�F�N�g�Ɠ����蔻��
		//��
		List<Wall>* wall = Wall::HitList();
		VALUE<Wall>* itr = wall->Begin();
		bool Hit = false;

		while (itr)
		{
			D3DXVECTOR3 *quad = itr->Data->Quad();
			D3DXVECTOR3 MousePos = D3DXVECTOR3(mouse->PosX(), mouse->PosY(), 1.0f);

			if (Collision::CircleQuad(MousePos, 1.0f, quad, 4))
			{
				Hit = true;
				SelectedObj = itr->Data;
			}

			itr = itr->_Next;
		}

		//�����Ȃ���Ԃ��N���b�N������Z���N�g����
		if (!Hit)
		{
			SelectedObj = nullptr;
		}
	}

	//�I�u�W�F�N�g���I������Ă���
	if (SelectedObj != nullptr)
	{
		float power = 1.0f;
		if (key->Press(DIK_LSHIFT)){ power *= 10.0f; }

		//����
		if (mouse->Press(Mouse::MOUSE_LEFT))SelectedObj->AddPos(D3DXVECTOR3(mouse->MoveX(), mouse->MoveY(), 0.0f));
		if (key->Repeat(DIK_W)){ SelectedObj->AddSizeY(1.0f*power); }
		if (key->Repeat(DIK_S)){ SelectedObj->AddSizeY(-1.0f*power); }
		if (key->Repeat(DIK_A)){ SelectedObj->AddSizeX(-1.0f*power); }
		if (key->Repeat(DIK_D)){ SelectedObj->AddSizeX(1.0f*power); }
		if (key->Repeat(DIK_Q)){ SelectedObj->AddRot(-0.003f*power); }
		if (key->Repeat(DIK_E)){ SelectedObj->AddRot(0.003f*power); }

		//���W�\��
		CDebugProc::Print("�I�u�W�F�N�g���W   X=%3f Y=%3f\n", SelectedObj->Pos().x, SelectedObj->Pos().y);
		CDebugProc::Print("�I�u�W�F�N�g�T�C�Y X=%3f Y=%3f\n", SelectedObj->Size().x, SelectedObj->Size().y);
		CDebugProc::Print("�I�u�W�F�N�g�̉�] %3f\n", SelectedObj->Rot().z);
	}
	else
	{
		if (mouse->Press(Mouse::MOUSE_LEFT))
		{
			Camera2D *cam = Camera2D::GetCamera(0);

			cam->AddPosP(D3DXVECTOR3(-mouse->MoveX() , 0.0f, 0.0f));
		}
	}
}
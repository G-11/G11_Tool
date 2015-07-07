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
#include "Item.h"
#include "Wall.h"
#include "Texture.h"
#include "Gimmick.h"
#include "StartDevice.h"
#include <stdio.h>


Tool* Tool::SelfPointer = new Tool;


Tool::Tool()
{
	SelectedObj = nullptr;
}

void Tool::Update()
{
	Mouse *mouse = VC::Instance()->mouse();
	Keyboard *key = VC::Instance()->keyboard();

	//�I�u�W�F�N�g���E�N���b�N�ō폜
	if (mouse->Trigger(Mouse::MOUSE_RIGHT))
	{
		//�I�u�W�F�N�g�Ɠ����蔻��

		D3DXVECTOR3 MousePos = D3DXVECTOR3(mouse->PosX(), mouse->PosY(), 1.0f);

		for (int i = 0; i < Sprite::LAYER_MAX; i++)
		{
			Sprite* Cur = Sprite::Top(i);
			Sprite* Next;
			
			while (Cur != NULL)
			{
				Next = Cur->Next();
				if (Cur->Type() == Sprite::OBJTYPE_FLOOR | 
					Cur->Type() == Sprite::OBJTYPE_GIMMICK | 
					Cur->Type() == Sprite::OBJTYPE_ITEM | 
					Cur->Type() == Sprite::OBJTYPE_WALL)
				{
					D3DXVECTOR3 *quad = Cur->Quad();
				

					if (Collision::CircleQuad(MousePos, 1.0f, quad, 4))
					{
						Cur->SetRelease();
						if (SelectedObj == Cur)SelectedObj = nullptr;
					}

				}
				Cur = Next;
			}
		}
		
	}
	
	//�N���b�N���ꂽ��
	if (mouse->Trigger(Mouse::MOUSE_LEFT))
	{
		//�I�u�W�F�N�g�Ɠ����蔻��
		bool Hit = false;

		//�I�u�W�F�N�g�Ɠ����蔻��

		D3DXVECTOR3 MousePos = D3DXVECTOR3(mouse->PosX(), mouse->PosY(), 1.0f);

		for (int i = 0; i < Sprite::LAYER_MAX; i++)
		{
			Sprite* Cur = Sprite::Top(i);
			Sprite* Next;

			while (Cur != NULL)
			{
				Next = Cur->Next();
				if (Cur->Type() == Sprite::OBJTYPE_FLOOR |
					Cur->Type() == Sprite::OBJTYPE_GIMMICK |
					Cur->Type() == Sprite::OBJTYPE_ITEM |
					Cur->Type() == Sprite::OBJTYPE_WALL)
				{
					D3DXVECTOR3 *quad = Cur->Quad();


					if (Collision::CircleQuad(MousePos, 1.0f, quad, 4))
					{
						Hit = true;
						SelectedObj = Cur;
						break;
					}

				}
				Cur = Next;
			}
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
		if (key->Repeat(DIK_UP)){ SelectedObj->AddPosY(-1.0f * power); }
		if (key->Repeat(DIK_DOWN)){ SelectedObj->AddPosY(1.0f * power); }
		if (key->Repeat(DIK_LEFT)){ SelectedObj->AddPosX(-1.0f * power); }
		if (key->Repeat(DIK_RIGHT)){ SelectedObj->AddPosX(1.0f * power); }
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


	//�I�u�W�F�N�g�̐���
	//�I�𒆂̃I�u�W�F�N�g�̃R�s�[
	if (key->Trigger(DIK_SPACE))
	{
		if (SelectedObj != nullptr)
		{
			SelectedObjectCopy(SelectedObj);
		}
	}

	//0�ŃX�v���C�g
	if (key->Trigger(DIK_0))
	{
		CreateObject(Sprite::OBJTYPE_SPRITE, D3DXVECTOR3(mouse->PosX(), mouse->PosY(), 0.0f), D3DXVECTOR2(50.0f, 50.0f), WHITE(1.0f), Sprite::LAYER_1);
	}

	//1�ŕǐ���
	if (key->Trigger(DIK_1))
	{
		CreateObject(Sprite::OBJTYPE_WALL, D3DXVECTOR3(mouse->PosX(), mouse->PosY(), 0.0f), D3DXVECTOR2(50.0f, 50.0f), WHITE(1.0f), Sprite::LAYER_1);
	}

	//2�ŃA�C�e��
	if (key->Trigger(DIK_2))
	{
		CreateObject(Sprite::OBJTYPE_ITEM, D3DXVECTOR3(mouse->PosX(), mouse->PosY(), 0.0f), D3DXVECTOR2(86.0f, 86.0f), WHITE(1.0f), Sprite::LAYER_1);
	}

	//3�ŃM�~�b�N
	if (key->Trigger(DIK_3))
	{
		CreateObject(Sprite::OBJTYPE_GIMMICK, D3DXVECTOR3(mouse->PosX(), mouse->PosY(), 0.0f), D3DXVECTOR2(50.0f, 50.0f), WHITE(1.0f), Sprite::LAYER_1);
	}

	//�G���^�[�L�[�ŃA�E�g�v�b�g
	if (key->Trigger(DIK_RETURN))
	{
		Output();
	}
}

//�S�f�[�^���v���O�����̌`���Ńe�L�X�g�o��
void Tool::Output()
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	char FileName[128] = {0};
	sprintf(FileName, "Map/Create_%d_%d_%d_%d.txt", time.wMonth, time.wDay, time.wHour, time.wMinute);

	FILE *fp;
	fp = fopen(FileName, "wt");

	fprintf(fp, "D3DXVECTOR3 *quad;\n\n");
	fprintf(fp, "Sprite* sp;\n\n");

	for (int i = 0; i < Sprite::LAYER_MAX; i++)
	{
		Sprite* Cur = Sprite::Top(i);
		Sprite* Next;

		

		while (Cur != NULL)
		{
			Next = Cur->Next();
			
			switch (Cur->Type())
			{
			case Sprite::OBJTYPE_WALL:
				fprintf(fp, "sp = Wall::Create(D3DXVECTOR2(%f, %f), D3DXVECTOR2(%f, %f), TEX_GOAL, Sprite::LAYER_1);\n", Cur->Pos().x, Cur->Pos().y, Cur->Size().x, Cur->Size().y);
				fprintf(fp, "sp->SetRot(D3DXVECTOR3(%f, %f, %f));\n", Cur->Rot().x, Cur->Rot().y, Cur->Rot().z);
				break;

			case Sprite::OBJTYPE_ITEM:
				fprintf(fp, "Item::Create(D3DXVECTOR3(%f, %f, %f), D3DXVECTOR2(%f, %f), WHITE(1.0f), ITEM_ID_USAGI);\n", Cur->Pos().x, Cur->Pos().y, Cur->Pos().z, Cur->Size().x, Cur->Size().y);
				break;

			case Sprite::OBJTYPE_GIMMICK:
				fprintf(fp, "sp = Gimmick::Create(D3DXVECTOR2(%f, %f), D3DXVECTOR2(%f, %f), Sprite::LAYER_1);\n", Cur->Pos().x, Cur->Pos().y, Cur->Size().x, Cur->Size().y);
				fprintf(fp, "sp->SetRot(D3DXVECTOR3(%f, %f, %f));\n", Cur->Rot().x, Cur->Rot().y, Cur->Rot().z);
				break;

			default:
				break;
			}

			Cur = Next;
		}
	}


	fclose(fp);
}


void Tool::CreateObject(Sprite::OBJTYPE type, const D3DXVECTOR3 &pos, const D3DXVECTOR2 &size, const D3DXCOLOR &color, int priority)
{
	switch (type)
	{
	case Sprite::OBJTYPE_SPRITE:
		Sprite::Create(pos, size, color, priority);
		break;

	case Sprite::OBJTYPE_ITEM:
		Item::Create(pos, size, color, ITEM_ID_USAGI, priority);
		break;

	case Sprite::OBJTYPE_WALL:
		Wall::Create(D3DXVECTOR2(pos.x, pos.y), size, TEX_TOOL_WALL);
		break;

	case Sprite::OBJTYPE_GIMMICK:
		Gimmick* gim;
		gim = Gimmick::Create(D3DXVECTOR2(pos.x, pos.y), size, priority);
		gim->SetTexture(GetTexture(TEX_TOOL_GIMMICK));
		break;

	case Sprite::OBJTYPE_ROOF:
		Wall::CreateFloor(D3DXVECTOR2(pos.x, pos.y), size, TEX_TOOL_ROOF);
		break;

	case Sprite::OBJTYPE_STARTDEVICE_ON:
		break;

	case Sprite::OBJTYPE_STARTDEVICE_SWITCH:
		break;

	case Sprite::OBJTYPE_STARTDEVICE_TIMER:
		break;

	case Sprite::OBJTYPE_STARTDEVICE_TRIGGER:
		break;

	default:
		break;

	}
}

void Tool::SelectedObjectCopy(Sprite* Selected)
{
	switch (Selected->Type())
	{
	case Sprite::OBJTYPE_SPRITE:
		Sprite* sp;
		sp = Sprite::Create(Selected->Pos(), D3DXVECTOR2(Selected->Size().x, Selected->Size().y), Selected->Color(), Sprite::LAYER_1);
		sp->SetRot(Selected->Rot());
		break;

	case Sprite::OBJTYPE_ITEM:
		Item* it;
		it = Item::Create(Selected->Pos(), D3DXVECTOR2(Selected->Size().x, Selected->Size().y), Selected->Color(), ITEM_ID_USAGI, Sprite::LAYER_1);
		it->SetRot(Selected->Rot());
		break;

	case Sprite::OBJTYPE_WALL:
		Wall* wall;
		wall = Wall::Create(D3DXVECTOR2(Selected->Pos().x, Selected->Pos().y), D3DXVECTOR2(Selected->Size().x, Selected->Size().y), TEX_TOOL_WALL);
		wall->SetRot(Selected->Rot());
		break;

	case Sprite::OBJTYPE_GIMMICK:
		Gimmick* gim;
		gim = Gimmick::Create(D3DXVECTOR2(Selected->Pos().x, Selected->Pos().y), D3DXVECTOR2(Selected->Size().x, Selected->Size().y), Sprite::LAYER_1);
		gim->SetTexture(GetTexture(TEX_TOOL_GIMMICK));
		gim->SetRot(Selected->Rot());
		break;

	case Sprite::OBJTYPE_ROOF:
		Wall::CreateFloor(D3DXVECTOR2(Selected->Pos().x, Selected->Pos().y), D3DXVECTOR2(Selected->Size().x, Selected->Size().y), TEX_TOOL_ROOF);
		break;

	case Sprite::OBJTYPE_STARTDEVICE_ON:
		break;

	case Sprite::OBJTYPE_STARTDEVICE_SWITCH:
		break;

	case Sprite::OBJTYPE_STARTDEVICE_TIMER:
		break;

	case Sprite::OBJTYPE_STARTDEVICE_TRIGGER:
		break;

	default:
		break;

	}
}
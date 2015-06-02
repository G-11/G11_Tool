//================================================================================
//	�A�C�e���N���X
//	Author:Tsukagoshi Toshiyuki
//================================================================================

//================================================================================
//	include
//================================================================================
#include "Item.h"
#include "Texture.h"
#include "Collision.h"

//================================================================================
//	�ÓI�����o
//================================================================================
List<Item> Item::_ItemList;

const D3DXVECTOR2 DEFAULT_ITEM_SIZE = D3DXVECTOR2(84.0f,84.0f);//�A�C�e���̃f�t�H���g�T�C�Y(���[����128x128�̎�)

//================================================================================
//	�A�C�e���e�[�u��
//================================================================================
const float Item::ItemScore[ITEM_ID_MAX] = {
	10.0f,			//������
	-5.0f,			//�˂�
	50.0f,			//����
};

//================================================================================
//	����
//================================================================================
Item* Item::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR2 &size, const D3DXCOLOR &color, const int &id, int priority)
{
	Item* item = new Item(priority);
	item->SetPos(pos);
	item->SetSize(D3DXVECTOR3(size.x, size.y, 0));
	item->SetColor(color);

	item->SetID(id);
	item->SetTexture(GetTexture(TEX((int)TEX_ITEM_OFFSET + id)));
	item->SetScore(ItemScore[id-1]);

	return item;
}

//================================================================================
//	�R���X�g���N�^
//================================================================================
Item::Item(int priority) :Sprite(priority)
{
	SelfIterator = nullptr;
	SelfIterator = _ItemList.Add(this);

	_State = ITEM_STATE_NON;
	EatedCount = 0.0f;
}

//================================================================================
//	�f�X�g���N�^
//================================================================================
Item::~Item()
{
	_ItemList.Delete(SelfIterator);
	SelfIterator = nullptr;
}

//================================================================================
//	�X�V����
//================================================================================
void Item::Update()
{
	//�H�ׂ��Ă��鎞�̃A�j���[�V����
	if (_State == ITEM_STATE_EATED)
	{
		D3DXVECTOR3 Vec;
		D3DXVECTOR3 Size;
		
		D3DXVec3Lerp(&Vec, PlayerPos, &_Pos, (1.0f - EatedCount / _CountMax));
		D3DXVec3Lerp(&Size, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &_Size, (1.0f - EatedCount / _CountMax));
		
		_Pos = Vec;
		_Size = Size;

		EatedCount += _AnimationSpeed;

		if (EatedCount > _CountMax)
		{
			SetRelease();
		}
	}

	Sprite::Update();
}

//================================================================================
//	�����蔻��
//	�A�C�e���ɂ������Ă����瓖�����Ă���A�C�e���̃|�C���^�[��Ԃ�
//	�������Ă��Ȃ�������k���|�C���^�[��Ԃ�
//================================================================================
Item* Item::HitCheck(const D3DXVECTOR3& Pos, const D3DXVECTOR3& Size)
{
	VALUE<Item>* itr = _ItemList.Begin();

	while (itr)
	{
		if (itr->Data->_State != ITEM_STATE_EATED)
		{
			if (Collision::Circle(Pos, Size.x*0.5f, itr->Data->_Pos, itr->Data->_Size.x*0.5f))
			{
				return itr->Data;
			}
		}
		itr = itr->_Next;
	}

	return nullptr;
}

//================================================================================
//	�H�ׂ�ꂽ���̃A�j���[�V����
//================================================================================
void Item::Action()
{

}

//================================================================================
//	�H�ׂ�ꂽ���̃A�j���[�V����
//================================================================================
void Item::Action(D3DXVECTOR3* Pos, float CountMax, float AnimationSpeed)
{
	Eated(Pos, CountMax, AnimationSpeed);
}

//================================================================================
//	�H�ׂ�ꂽ���̃A�j���[�V����
//================================================================================
void Item::Eated(D3DXVECTOR3* Pos, float CountMax, float AnimationSpeed)
{
	_State = ITEM_STATE_EATED;
	PlayerPos = Pos;
	_CountMax = CountMax;
	_AnimationSpeed = AnimationSpeed;
}
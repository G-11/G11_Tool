#pragma once
#ifndef _TOOL_H_
#define _TOOL_H_

#include "Sprite.h"
class Sprite;

class Tool
{
public :
	~Tool(){}
	static Tool* GetInstance(){ return SelfPointer; }
	void Update();

	void AddPos();		//�I������Ă���I�u�W�F�N�g�̈ړ�
	void AddSize();		//�I������Ă���I�u�W�F�N�g�̊g�k
	void AddRot();		//�I������Ă���I�u�W�F�N�g�̉�]

	void Output();		//��������Ă���I�u�W�F�N�g���o��

	void SelectedObjectCopy(Sprite* Selected);
	void CreateObject(Sprite::OBJTYPE type, const D3DXVECTOR3 &pos, const D3DXVECTOR2 &size, const D3DXCOLOR &color, int priority);

private:
	Tool();
	static Tool* SelfPointer;

	Sprite* SelectedObj;
};

#endif
#pragma once
#ifndef _TOOL_H_
#define _TOOL_H_

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

private:
	Tool();
	static Tool* SelfPointer;

	Sprite* SelectedObj;
};

#endif
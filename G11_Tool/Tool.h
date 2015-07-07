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

	void AddPos();		//選択されているオブジェクトの移動
	void AddSize();		//選択されているオブジェクトの拡縮
	void AddRot();		//選択されているオブジェクトの回転

	void Output();		//生成されているオブジェクトを出力

	void SelectedObjectCopy(Sprite* Selected);
	void CreateObject(Sprite::OBJTYPE type, const D3DXVECTOR3 &pos, const D3DXVECTOR2 &size, const D3DXCOLOR &color, int priority);

private:
	Tool();
	static Tool* SelfPointer;

	Sprite* SelectedObj;
};

#endif
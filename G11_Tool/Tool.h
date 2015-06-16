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

	void AddPos();		//選択されているオブジェクトの移動
	void AddSize();		//選択されているオブジェクトの拡縮
	void AddRot();		//選択されているオブジェクトの回転

private:
	Tool();
	static Tool* SelfPointer;

	Sprite* SelectedObj;
};

#endif
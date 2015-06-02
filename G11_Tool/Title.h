//
//Title.h
//
#ifndef _TITLE_H_
#define _TITLE_H_
#include "main.h"
#include "Scene.h"
class Sprite;
class Eatan;

class Title :public Scene
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

private:

	Sprite *Bg;//�w�i

	Sprite *Usagi;//�E�T�M�L����
	Sprite *Kuma;//�F�L����
	Sprite *Neko;//�l�R�L����
	Eatan *TEatan;//���[����

	Sprite *Logo;//�_�ŗp�摜
	Sprite *Start;//
	Sprite *Tutorial;
	Sprite *Collection;
	Sprite *Config;

	
	float Flahing,AddFlashingNum,RotFream;//���l,������l,���E���p
	int MenuNum;

#if _DEBUG
	float test_num;
#endif

};

#endif
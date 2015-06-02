//
//Result.h
//
#ifndef _RESULT_H_
#define _RESULT_H_
#include "main.h"
#include "Scene.h"
#include "Item.h"
class Sprite;
class Gauge;


typedef struct 
{
	int id;
	bool vis;
}VIS_CHAR;


class Result :public Scene
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetResultCharId(int id);
private:
	Sprite *BG;//�w�i
	Sprite *Push;//Push�{�^��
	Sprite *REatan;//���U���g�p���[����
	Sprite *CharFream;//�R���N�V�����p�L�����g

	Sprite *ResultChar[6];//�L�����N�^��

	Sprite *GaugeFream;//�Q�[�W�O�g
	Gauge *GlowGauge;//�Q�[�W

	static VIS_CHAR ResultCharId[6];//�L�����̕\����\��
	float flahing, add_flahing_num;//���l,������l 
	bool charFlag;//�L�����N�^��
	int charVisTime;//�擾�L�����\������
	int charVisCnt;
	void ResetResultChar(void);

};

#endif
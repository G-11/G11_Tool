#pragma once
#ifndef _CHECK_POINT_H_
#define _CHECK_POINT_H_
#include "Sprite.h"

class CheckPoint :public Sprite
{
public:
	CheckPoint(int priority = Sprite::LAYER_4);
	virtual ~CheckPoint();
	static CheckPoint* Create(const D3DXVECTOR2& pos);

	void Update(void);

	bool Active(void)const { return _Active; }

	void SwitchActive(void){//アクティブフラグがON・OFF切り替わる
		(_Active)? uv.x = 0: uv.x = uv.z;
		(_Active)? _Active = false : _Active = true;
	}

private:
	static List<CheckPoint> _HitList;
	VALUE<CheckPoint>* SelfIterator;
	bool _Active;

	const static D3DXVECTOR2 DefaultSize;

};

#endif
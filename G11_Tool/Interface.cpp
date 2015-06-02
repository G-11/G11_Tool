#include "Interface.h"
#include "Gauge.h"
#include "Sprite.h"

float Interface::_Score = 0;

Interface::Interface()
{
	GlowGauge = nullptr;
	GaugeFrame = nullptr;
	Clock = nullptr;
	ClockNeedle = nullptr;

	TimeLimit = Time = 0;
	_Score = 0;
}

Interface::~Interface()
{
	
}

void Interface::Init(int timeLimit,int gaugeMax)
{
	TimeLimit = (float)timeLimit;
	Clock = Sprite::Create(D3DXVECTOR3(74.0f,74.0f,0),D3DXVECTOR2(128.0f,128.0f),WHITE(1.0f),Sprite::LAYER_INTERFACE);
	Clock->SetTexture(GetTexture(TEX_CLOCK));
	ClockNeedle = Sprite::Create(D3DXVECTOR3(74.0f,74.0f,0),D3DXVECTOR2(128.0f,128.0f),WHITE(1.0f),Sprite::LAYER_INTERFACE);
	ClockNeedle->SetTexture(GetTexture(TEX_CLOCK_NEEDLE));
	
	GlowGauge = Gauge::Create(D3DXVECTOR2(74.0f,656.0f),D3DXVECTOR2(128.0f,512.0f),0,(float)gaugeMax,0,UP,Sprite::LAYER_INTERFACE);
	GlowGauge->SetDelay(0.02f);
	GlowGauge->SetTexture(GetTexture(TEX_GLOW_GAUGE));

	GaugeFrame = Sprite::Create(D3DXVECTOR3(74.0f,400.0f,0),D3DXVECTOR2(128.0f,512.0f),WHITE(1.0f),Sprite::LAYER_INTERFACE);
	GaugeFrame->SetTexture(GetTexture(TEX_GLOE_GAUGE_FRAME));
}

void Interface::Update(void)
{
	_Percent = (float)(Time / TimeLimit);

	float Angle = _Percent * (PI * 2);

	ClockNeedle->SetRot(Angle);
	GlowGauge->SetCurrent(_Score);
	if (Time < TimeLimit)
	{
		Time++;
	}
}
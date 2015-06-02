#include "Option.h"
#include "Sprite.h"
#include "Gauge.h"
#include "Font.h"
#include "Fade.h"
#include "Manager.h"

const short Option::ItemMax = 3;

Option::Option()
{
	BGMFrame = SEFrame = nullptr;
	BGMGauge = SEGauge = nullptr;
	Cursor = nullptr;

	Select = 0;
}

void Option::Init(void)
{
	
	Font::SetPenSize(10);

	Sprite* bg = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f,0),D3DXVECTOR2(SCREEN_WIDTH,SCREEN_HEIGHT),WHITE(1.0f),Sprite::LAYER_BACKGROUND);
	bg->SetTexture(GetTexture(TEX_FIELD1_BG));

	String* string = String::Create(D3DXVECTOR2(SCREEN_WIDTH / 2.0f,70.0f),80.0f,"�����Ă�");
	string->SetPos(D3DXVECTOR2(SCREEN_WIDTH / 2.0f,70.0f));

	float bgm = Sound::MVolumeBGM();
	BGMGauge = Gauge::Create(D3DXVECTOR2(384.0f,200.0f),D3DXVECTOR2(128.0f,512.0f),0,1.0f,bgm,UP,Sprite::LAYER_1);
	BGMGauge->SetRot(DEG2RAD(90.0f));
	BGMGauge->SetTexture(GetTexture(TEX_GLOW_GAUGE));

	CursolPosition[0] = BGMGauge->Pos();
	CursolPosition[0].x -= 50.0f;

	BGMFrame = Sprite::Create(D3DXVECTOR3(640.0f,200.0f,0),D3DXVECTOR2(128.0f,512.0f),WHITE(1.0f),Sprite::LAYER_2);
	BGMFrame->SetRot(DEG2RAD(90.0f));
	BGMFrame->SetTexture(GetTexture(TEX_GLOE_GAUGE_FRAME));

	float se = Sound::MVolumeSE();
	SEGauge = Gauge::Create(D3DXVECTOR2(384.0f,350.0f),D3DXVECTOR2(128.0f,512.0f),0,1.0f,se,UP,Sprite::LAYER_1);
	SEGauge->SetRot(DEG2RAD(90.0f));
	SEGauge->SetTexture(GetTexture(TEX_GLOW_GAUGE));

	CursolPosition[1] = SEGauge->Pos();
	CursolPosition[1].x -= 50.0f;

	SEFrame = Sprite::Create(D3DXVECTOR3(640.0f,350.0f,0),D3DXVECTOR2(128.0f,512.0f),WHITE(1.0f),Sprite::LAYER_2);
	SEFrame->SetRot(DEG2RAD(90.0f));
	SEFrame->SetTexture(GetTexture(TEX_GLOE_GAUGE_FRAME));

	String* exit = String::Create(D3DXVECTOR2(640.0f,600.0f),80.0f,"���ǂ�");
	exit->SetPos(D3DXVECTOR2(640.0f,500.0f));


	CursolPosition[2] = D3DXVECTOR3(450.0f,500.0f,0);

	Cursor = Sprite::Create(D3DXVECTOR3(BGMGauge->Pos().x - 150.0f,BGMGauge->Pos().y,0),D3DXVECTOR2(128.0f,128.0f),WHITE(1.0f),Sprite::LAYER_3);
	Cursor->SetTexture(GetTexture(TEX_ITEM_USAGI));

	Sound::Instance()->Play(BGM_TITLE);
	Font::SetPenSize(5);

}

void Option::Uninit(void)
{
	Manager::ReleaseObject();
}

void Option::Update(void)
{
	D3DXVECTOR3 pos = Cursor->Pos();
	pos = (DestPos-pos)*0.1f;
	Cursor->AddPos(pos);

	float bgm = Sound::MVolumeBGM();
	float se = Sound::MVolumeSE();
	BGMGauge->SetCurrent(bgm);
	SEGauge->SetCurrent(se);
	VC* vc = VC::Instance();
	frame++;
	float offsetY = sinf(DEG2RAD(frame))*0.2f;
	Cursor->SetOffsetY(offsetY);
	if (vc->Trigger(COMMAND_UP))
	{
		Select--;
		if (Select < 0)
		{
			Select = ItemMax-1;
		}
	}
	else if (vc->Trigger(COMMAND_DOWN))
	{
		Select++;
		Select %= ItemMax;
	}
	DestPos = CursolPosition[Select];
	switch (Select)
	{
	case 0:
		if (vc->Repeat(COMMAND_LEFT))
		{
			Sound::AddMasterVolumeBGM(-0.1f);
		}
		else if (vc->Repeat(COMMAND_RIGHT))
		{
			Sound::AddMasterVolumeBGM(0.1f);
		}
		break;
	case 1:
		if (vc->Repeat(COMMAND_LEFT))
		{
			Sound::AddMasterVolumeSE(-0.1f);
		}
		else if (vc->Repeat(COMMAND_RIGHT))
		{
			Sound::AddMasterVolumeSE(0.1f);
		}
		if (vc->Trigger(COMMAND_OK))
		{
			Sound::Instance()->Play(SE_JINGLE);
		}
		break;
	case 2:
		if (vc->Trigger(COMMAND_OK) && Fade::Instance()->Mode() == Fade::FADE_NONE)
		{
			Manager::Instance()->SetScene(Manager::SCENE_TITLE);
		}
		break;
	default:
		break;
	}
}
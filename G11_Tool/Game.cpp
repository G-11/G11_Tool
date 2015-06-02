#include "Game.h"
#include "Sprite.h"
#include "Gauge.h"
#include "Camera2D.h"
#include "Light2D.h"
#include "Renderer.h"
#include "Manager.h"
#include "Pause.h"
#include "Shader2D.h"

#include "Player.h"
#include "Item.h"
#include "Interface.h"

#include "Wall.h"
#include "ScreenRender.h"

#include "StartDevice.h"
#include "BreakObject.h"
#include "BoundObject.h"
#include "CheckPoint.h"

#include "Stage.h"
#include "Stage_1.h"
#include "Goal.h"
#include "Input\Mouse.h"

#ifdef _DEBUG
#include "Input/Keyboard.h"


bool Game::Hit = false;

#endif

bool Game::_PauseFlag = false;

Sprite	*Game::_field = nullptr;
Wall	*Game::_cilling = nullptr;
Wall	*Game::_floor = nullptr;

Player	*Game::_player = nullptr;
Item	*Game::_item = nullptr;
Stage	*Game::_stage = nullptr;

float time = 0;

void Game::Init(void)
{

	Window* window = Window::Instance();
	Sound::Instance()->Play(BGM_TITLE);
	
	// 背景
	_field = Sprite::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), WHITE(1.0f), Sprite::LAYER_BACKGROUND);
	_field->SetTexture(GetTexture(TEX_FIELD1_BG));

}

void Game::Uninit(void)
{
	Camera2D* camera = Camera2D::GetCamera(0);
	camera->SetPlayer(nullptr);
	
	StartDevice::SetPlayer(nullptr);
	BreakObject::SetPlayer(nullptr);
	SafeDelete(_Interface);
	Renderer::SetRenderMode(CScreenRender::MODE_NORMAL);
	Sound::Instance()->Fade(100);
	Manager::ReleaseObject();
}

void Game::Update(void)
{
	if (_stage != nullptr)
	{
		_stage->Update();
	}

	//_Interface->Update();
	//_field->SetMaskUVX(_Interface->Percent());

	_field->AddUVX(Camera2D::GetCamera(0)->GetSpeed().x*-0.00001);

	if (Pause != nullptr)
	{
		Pause->Update();
	}

#ifdef _DEBUG

	CDebugProc::Print("0 リザルトへ遷移\n");
	if (VC::Instance()->keyboard()->Trigger(DIK_0) &&
		Fade::Instance()->Mode() == Fade::FADE_NONE)
	{
		Manager::Instance()->SetScene(Manager::SCENE_RESULT);
	}

	//アイテム生成
	if (VC::Instance()->keyboard()->Trigger(DIK_F1))
	{
		Item::Create(_player->Pos(), D3DXVECTOR2(84.0f, 84.0f), WHITE(1.0f), Randi(1, 3), Sprite::LAYER_2);
	}

	//ゲージリセット
	if (VC::Instance()->keyboard()->Trigger(DIK_F2))
	{
		Interface::AddScore(-Interface::Score());
	}

	//ゲームリセット
	if (VC::Instance()->keyboard()->Trigger(DIK_9) &&
		Fade::Instance()->Mode() == Fade::FADE_NONE)
	{
		Manager::Instance()->SetScene(Manager::SCENE_GAME);
	}

#endif

}

void Game::Draw(void)
{


}
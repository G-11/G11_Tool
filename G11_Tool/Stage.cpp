#include "Stage.h"
#include "main.h"

#include "Stage_1.h"
//#include "Stage_2.h"
//#include "Stage_3.h"

STAGE_MODE Stage::stageMode = STAGE_MODE1;
Stage *Stage::_stageSelect = nullptr;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
Stage::Stage()
{
}

//=============================================================================
//����
//=============================================================================
Stage* Stage::Create(STAGE_MODE mode)
{

	return nullptr;
}

//=============================================================================
//�ǂ̃X�e�[�W�i�M�~�b�N�j��
//=============================================================================
void Stage::SetStage(STAGE_MODE mode)
{
	if (stageMode == mode)
		return;

	if (_stageSelect != nullptr)
		SafeRelease(_stageSelect);

	switch (mode)
	{
	case STAGE_MODE1:
		_stageSelect = new Stage_1;
		break;

	//case STAGE_MODE2:
		//_stageSelect = new Stage_2;
		//	break;

	//case STAGE_MODE3:
		//_stageSelect = new Stage_3;
		//	break;

	}
}
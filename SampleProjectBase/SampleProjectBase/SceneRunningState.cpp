#include "SceneRunningState.h"

SceneRunningState::SceneRunningState()
{
	// ������
	Init();
}

SceneRunningState::~SceneRunningState()
{
}

void SceneRunningState::Init()
{
	// �ғ���Ԃɂ���
	runState = RUNNING_STATE::RUNNING;
}

void SceneRunningState::SetFinshState()
{
	// �I����Ԃɂ���
	runState = RUNNING_STATE::FINISH;
}

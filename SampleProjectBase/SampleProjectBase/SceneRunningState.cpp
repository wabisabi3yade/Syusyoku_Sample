#include "SceneRunningState.h"

SceneRunningState::SceneRunningState()
{
	// 初期化
	Init();
}

SceneRunningState::~SceneRunningState()
{
}

void SceneRunningState::Init()
{
	// 稼働状態にする
	runState = RUNNING_STATE::RUNNING;
}

void SceneRunningState::SetFinshState()
{
	// 終了状態にする
	runState = RUNNING_STATE::FINISH;
}

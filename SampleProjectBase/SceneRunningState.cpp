#include "SceneRunningState.h"

SceneRunningState::SceneRunningState()
{
	// ‰Šú‰»
	Init();
}

SceneRunningState::~SceneRunningState()
{
}

void SceneRunningState::Init()
{
	// ‰Ò“­ó‘Ô‚É‚·‚é
	runState = RUNNING_STATE::RUNNING;
}

void SceneRunningState::SetFinshState()
{
	// I—¹ó‘Ô‚É‚·‚é
	runState = RUNNING_STATE::FINISH;
}

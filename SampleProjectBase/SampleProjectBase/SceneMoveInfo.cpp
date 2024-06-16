#include "SceneMoveInfo.h"

constexpr int SUBSCENE_TYPE_MIN = 0;	// サブシーンタイプの最低値
constexpr int SUBSCENE_TYPE_MAX = 256;	// サブシーンタイプの最大値
										// (何個あるかは各大局クラスによって変わる)

SceneMoveInfo::SceneMoveInfo(int _initSub, BroadType::Type _initBroad)
{
	// 初期のシーン情報を代入
	broadType = _initBroad;
	subType = _initSub;

	pRunningState = new SceneRunningState();
}

SceneMoveInfo::~SceneMoveInfo()
{
	CLASS_DELETE(pRunningState);
}

void SceneMoveInfo::OnChangeScene(int _nextSub, BroadType::Type _nextBroad)
{
	// 遷移先の大局・サブシーンのタイプを代入
	subType = _nextSub;
	broadType = _nextBroad;

	// シーンの稼働状態を終了にする
	pRunningState->SetFinshState();

}


int SceneMoveInfo::GetRunningState() const
{
	return pRunningState->GetRunningState();
}

void SceneMoveInfo::Init()
{
	pRunningState->Init();
}


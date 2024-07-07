#include "pch.h"
#include "Tank_ChangeSub.h"
// サブシーン
#include "Tank_InGameSub.h"

Tank_ChangeSub::Tank_ChangeSub(SceneMoveInfo* _moveInfo) : ChangeSubScene(_moveInfo)
{
}

Tank_ChangeSub::~Tank_ChangeSub()
{
}

SubScene_Base* Tank_ChangeSub::ChangeScene()
{
	// 先に前の解放する
	CLASS_DELETE(pSubScene);

	// 次のサブシーンのidを取得
	int nextSubId = moveInfo->GetNextSubType();

	// 遷移先のサブシーンに対応したシーンを作成する
	switch (nextSubId)
	{
	case Scene::InGame:
		pSubScene = new Tank_InGameSub(moveInfo);
		break;

	default:
		break;
	}

	return pSubScene;
}

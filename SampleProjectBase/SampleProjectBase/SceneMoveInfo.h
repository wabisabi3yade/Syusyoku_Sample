#pragma once
#include "BroadSceneType.h"
#include "SceneRunningState.h"


// シーン遷移に必要な情報（遷移先、遷移するかどうか）を扱うクラス
class SceneMoveInfo
{
	SceneRunningState* pRunningState;	// シーンの稼働状態（FINISHで遷移する）
	BROAD_TYPE::TYPE broadType;	// 遷移先の大局シーンタイプ
	int subType;	// 次のサブシーンのタイプ

public:
	// 引数：最初に呼ぶ	①サブシーン　②大局シーン
	SceneMoveInfo(int _initSub, BROAD_TYPE::TYPE _initBroad);
	~SceneMoveInfo();

	// シーン遷移するときに呼び出される
	void OnChangeScene(int _nextSub, BROAD_TYPE::TYPE _nextBroad);

	// 遷移先の大局シーンのタイプを取得
	BROAD_TYPE::TYPE GetNextBroadType()const { return broadType; }

	// 遷移先のサブシーンのタイプを取得
	int GetNextSubType()const { return subType; }

	// シーンの稼働状態を取得
	int GetRunningState()const;

	// シーン遷移後などに初期化する関数
	void Init();
};
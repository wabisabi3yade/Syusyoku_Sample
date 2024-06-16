#pragma once
#include "SceneRunningState.h"

namespace BroadType
{
	// 大局シーンの種類列挙型
	enum Type
	{
		None = -1,	// なにもなし
		Test,	// テスト
		Tank,	// タンクゲーム
		Num
	};
}

// シーン遷移に必要な情報（遷移先、遷移するかどうか）を扱うクラス
class SceneMoveInfo
{
	SceneRunningState* pRunningState;	// シーンの稼働状態（FINISHで遷移する）
	BroadType::Type broadType;	// 遷移先の大局シーンタイプ
	int subType;	// 次のサブシーンのタイプ

public:
	// 引数：最初に呼ぶ	①サブシーン　②大局シーン
	SceneMoveInfo(int _initSub, BroadType::Type _initBroad);
	~SceneMoveInfo();

	// シーン遷移するときに呼び出される
	void OnChangeScene(int _nextSub, BroadType::Type _nextBroad);

	// 遷移先の大局シーンのタイプを取得
	BroadType::Type GetNextBroadType()const { return broadType; }

	// 遷移先のサブシーンのタイプを取得
	int GetNextSubType()const { return subType; }

	// シーンの稼働状態を取得
	int GetRunningState()const;

	// シーン遷移後などに初期化する関数
	void Init();
};
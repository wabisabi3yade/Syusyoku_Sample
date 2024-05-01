#pragma once
#include "BroadSceneType.h"
class BroadScene_Base;
class SceneMoveInfo;

// 大局シーンを変更するクラス
class ChangeBroadScene
{
	BroadScene_Base* pBroadScene{nullptr};	// 大局シーン変数
	SceneMoveInfo* pMoveInfo{ nullptr };	// シーン遷移情報

	BROAD_TYPE::TYPE c_BroadId;	// 現在の大局シーンのid

public:
	ChangeBroadScene(SceneMoveInfo* _moveInfo);
	~ChangeBroadScene();

	// 大局シーンを変更する
	BroadScene_Base* OnChangeBroad();

	// 解放処理
	void Release();

};


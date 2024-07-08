#pragma once
#include "SceneMoveInfo.h"

class BroadScene_Base;
class SceneMoveInfo;

// 大局シーンを変更するクラス
class ChangeBroadScene
{
	BroadScene_Base* pBroadScene{nullptr};	// 大局シーン変数
	SceneMoveInfo* pMoveInfo{ nullptr };	// シーン遷移情報

	BroadType::Type c_BroadId;	// 現在の大局シーンのid

public:
	ChangeBroadScene(SceneMoveInfo* _moveInfo);
	~ChangeBroadScene();

	// 大局シーンを変更する
	BroadScene_Base* OnChangeBroad();

	// 解放処理
	void Release();

};


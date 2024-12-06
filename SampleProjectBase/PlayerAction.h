#pragma once
#include "PlayerActionController_Base.h"
#include "ITargetAccepter.h"

/// @brief プレイヤーアクションクラス
class PlayerAction
{
	/// @brief 入力クラス
	GameInput* pInput;

	/// @brief バトルマネジャー
	CP_BattleManager* pBattleManager;

	/// @brief ターゲット先をカメラに伝える
	IObjectTargeter* pTargetCamera;

	/// @brief カメラ
	CP_Camera* pCamera;

	/// @brief キャンセルフラグのポインタ
	const bool* pIsCanCancel;

	/// @brief 入力可能フラグのポインタ
	const bool* pIsCanInput;

	/// @brief 攻撃可能のポインタ
	const bool* pIsCanAttack;

	/// @brief 移動可能のポインタ
	const bool* pIsCanMove;

	/// @brief ターゲット中かどうか
	bool isTargeting;

	/// @brief 1フレーム前のターゲット状態
	bool prevIsTargeting;
public:
	PlayerAction();
	~PlayerAction() {}
};


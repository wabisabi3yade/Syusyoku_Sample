#pragma once
#include "PlayerActState_Base.h"

class PlayerAirActionController;

/// @brief プレイヤー空中行動のベース
class PlayerAirState : public PlayerActState_Base
{
public:
	/// @brief  プレイヤーの行動状態
	enum class PlayerState
	{
		// 無し
		None = 99,

		// 待機
		Idle = 100,

		// 移動
		Move,
		Damage,	// ダメージ大のけぞり
		Guard,	// ガード開始

		// コンビネーション攻撃
		Attack11 = 120,
		Attack12,
		Attack13,
		Attack14,

		// 必殺攻撃
		HelmBreaker = 140,	// 兜割り
		SpecialAtkGuard,	// ガードパリィ時の攻撃
	};

protected:
	/// @brief 各状態の開始処理
	virtual void OnStartBehavior() {};

	/// @brief 更新処理
	virtual void UpdateBehavior() {};

	/// @brief 各状態の終了処理
	virtual void OnEndBehavior() {};

	/// @brief ステート遷移条件のチェック処理
	virtual void TransitionCheckUpdate();

	/// @brief 空中行動のコントローラーにキャスト
	/// @return 空中行動コントローラー
	PlayerAirActionController& CastAirController();

	/// @brief 状態を遷移する
	/// @param _changeSate 遷移先の状態
	/// @param _isForce) 強制的に
	void ChangeState(PlayerState _nextState, bool _isForce = false);

	void ImGuiDebug() override;
};


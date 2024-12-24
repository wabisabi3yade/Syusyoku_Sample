#pragma once
#include "PlayerActState_Base.h"

namespace HashiTaku
{
	class PlayerGroundActionController;

	/// @brief 地上行動の基底クラス
	class PlayerGroundState : public PlayerActState_Base
	{
	public:
		/// @brief  プレイヤーの行動状態
		enum class PlayerState
		{
			// 待機
			Idle,

			// 移動
			Move,
			TargetMove,
			BeginJump,
			Rolling,
			Damage_S,	// ダメージ小のけぞり
			Damage_L,	// ダメージ大のけぞり
			Guard,	// ガード開始
			Guard_Parry,	// パリィ処理
			Landing,	// 着地

			// 地上コンビネーション攻撃
			Attack11 = 20,
			Attack12,
			Attack13,
			Attack14,

			// 地上必殺攻撃
			SpecialAtkHi = 30,
			SpecialAtkLow,
			SpecialAtkGuard,	// ガードパリィ時の攻撃
			ChargeAttack1,	// 溜め攻撃

			// 最後
			None = 99
		};
	public:
		PlayerGroundState();
		virtual  ~PlayerGroundState() {}

		/// @brief 開始処理呼び出し
		void OnStart() override;

		/// @brief 更新処理呼び出し
		void Update() override;

		/// @brief  終了処理呼び出し
		void OnEnd() override;

		json Save() override;
		void Load(const json& _data) override;
	protected:
		/// @brief 各状態の開始処理
		virtual void OnStartBehavior() {};

		/// @brief 更新処理
		virtual void UpdateBehavior() {};

		/// @brief 各状態の終了処理
		virtual void OnEndBehavior() {};

		/// @brief ステート遷移条件のチェック処理
		virtual void TransitionCheckUpdate() {}

		/// @brief 状態を遷移する
		/// @param _changeSate 遷移先の状態
		/// @param _isForce) 強制的に
		void ChangeState(PlayerState _nextState, bool _isForce = false);

		/// @brief 地上行動コントローラーに変換
		PlayerGroundActionController& CastGroundController();

	private:
		/// @brief 入力更新
		void InputUpdate();
	};
}
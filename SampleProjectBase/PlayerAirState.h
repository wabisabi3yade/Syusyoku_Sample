#pragma once
#include "PlayerActState_Base.h"

namespace HashiTaku
{
	class PlayerAirActionController;

	/// @brief プレイヤー空中行動のベース
	class PlayerAirState : public PlayerActState_Base
	{
	public:
		/// @brief  プレイヤーの行動状態
		enum class PlayerState
		{
			// 待機
			Idle = 1,

			// 移動
			Move,
			Damage,	// ダメージのけぞり
			Guard,	// ガード開始

			// コンビネーション攻撃
			Attack11 = 20,
			Attack12,
			Attack13,

			// 必殺攻撃
			HelmBreaker = 40,	// 兜割り

			// 空中切り上げ
			SlashHigh_Air,

			// 無し
			None = 99
		};

		/// @brief 下への力を反映させるか？
		bool isApplyDownForce;

	public:
		PlayerAirState();
		virtual ~PlayerAirState(){}

		/// @brief 下への力を反映させるか取得
		/// @return 下への力を取得する
		bool GetIsApplyDownForce() const;

		/// @brief ステート側のダメージ処理
		/// @param _attackInfo 受けた攻撃情報
		/// @return ダメージ処理を行うか？
		bool OnDamage(AttackInformation& _attackInfo) override;

	protected:
		/// @brief 開始処理呼び出し
		void OnStart() override;

		/// @brief 更新処理呼び出し
		void Update() override;

		/// @brief  終了処理呼び出し
		void OnEnd() override;

		/// @brief 各状態の開始処理
		virtual void OnStartBehavior() {};

		/// @brief 各状態の更新処理
		virtual void UpdateBehavior() {};

		/// @brief 各状態の終了処理
		virtual void OnEndBehavior() {};

		/// @brief ステート遷移条件のチェック処理
		virtual void TransitionCheckUpdate();

		/// @brief 下への力を反映させるかセット
		/// @param _isDownForce 下への力を反映させるか?
		void SetIsApplyDownForce(bool _isDownForce);

		/// @brief 空中行動のコントローラーにキャスト
		/// @return 空中行動コントローラー
		PlayerAirActionController& GetAirController();

		/// @brief 状態を遷移する
		/// @param _changeSate 遷移先の状態
		/// @param _isForce) 強制的に
		void ChangeState(PlayerState _nextState, bool _isForce = false);

		void ImGuiDebug() override;

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief 入力更新
		void InputStateUpdate();

	protected:
		// 空中行動から戻るとき先アニメーション名
		static constexpr auto FALLLOOP_ANIM_NAME{ "Fall_Loop" };
	};
}
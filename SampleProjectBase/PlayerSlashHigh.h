#pragma once
#include "PlayerAttackState.h"

namespace HashiTaku
{
	/// @brief 地上切り上げ攻撃
	class PlayerGroundSlashHigh : public PlayerAttackState
	{
		/// @brief ジャンプの移動量カーブ
		AnimationCurve jumpMovementCurve;

		/// @brief ジャンプの移動量
		float maxJumpMovement;

		/// @brief ジャンプの力
		float jumpPower;

		/// @brief ジャンプするか判断するタイミングの割合
		float judgeTimeAnimRatio;

		/// @brief ジャンプするタイミングの割合
		float jumpTimeAnimRatio;

		/// @brief 1フレーム前のアニメーション割合
		float prevAnimRatio;

		/// @brief 1フレーム前のジャンプの移動量
		float prevJumpMovement;
		
		/// @brief 既に判断済みか？
		bool isAlreadyJudge;

		/// @brief ジャンプするのか？
		bool isWillJump;
	public:
		PlayerGroundSlashHigh();
		~PlayerGroundSlashHigh() {}

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief 開始処理
		void OnStartBehavior() override;

		/// @brief 更新する
		void UpdateBehavior() override;

		/// @brief ジャンプ移動更新処理
		void CheckBeginJumpUpdate();

		/// @brief ジャンプ切り替え更新チェック
		void CheckJumpUpdate();

		/// @brief ジャンプできるか取得する
		/// @param 現在のアニメーションの割合
		/// @return ジャンプできるか？
		bool CanJudge(float _curAnimRatio);

		/// @brief ジャンプ時の処理
		void OnJump();

		/// @brief ジャンプするかジャッジ
		void OnJudgeTransJump();

		void ImGuiDebug() override;
	};
}


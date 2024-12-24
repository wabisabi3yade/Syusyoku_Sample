#pragma once
#include"PlayerGroundState.h"

namespace HashiTaku
{
	/// @brief プレイヤージャンプステート
	class PlayerJumpState : public PlayerGroundState
	{
		/// @brief ジャンプの力
		float jumpPower;

		/// @brief ジャンプ開始までのかがむ時間
		float crouchTime;

		/// @brief ステート開始からの経過時間
		float stateElapsedTime;

		/// @brief  ジャンプしたかどうか？
		bool isAlreadyJump;
	public:
		PlayerJumpState();
		~PlayerJumpState();

		json Save() override;
		void Load(const json& _data) override;
	private:
		void OnStartBehavior() override;
		void UpdateBehavior() override;
		void OnEndBehavior() override;

		/// @brief ジャンプ開始するときの処理
		void OnBeginJump();

		/// @brief 入力方向に即時に向ける
		void LookInputVectorInstant();

		void ImGuiDebug() override;
	};
}
#pragma once
#include "PlayerAirState.h"

namespace HashiTaku
{
	/// @brief 空中移動
	class PlayerAirMove : public PlayerAirState
	{
		/// @brief 移動方向
		DXSimp::Vector3 moveVector;

		/// @brief 現在の速度
		float currentSpeed;

		/// @brief 最大速度
		float maxSpeed;

		/// @brief 加速度
		float acceleration;

		/// @brief 減速スピード
		float decadeSpeed;

		/// @brief 回転速度
		float vectorRotateSpeed;

		/// @brief ジャンプで上がり続ける最大時間
		float maxJumpUpTime;

		/// @brief ステート開始時の上にむけたジャンプスピード
		float enterJumpSpeed;
	public:
		PlayerAirMove();
		~PlayerAirMove() {}

		json Save() override;
		void Load(const json& _data) override;

	private:
		/// @brief 開始処理
		void OnStartBehavior() override;

		/// @brief 更新
		void UpdateBehavior() override;

		/// @brief 移動
		void Move();

		/// @brief 回転させる
		void Rotation();

		void ImGuiDebug() override;
	};
}
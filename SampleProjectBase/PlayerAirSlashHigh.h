#pragma once
#include "PlayerAirState.h"

namespace HashiTaku
{
	/// @brief 斬り上げで空中に行ったステート
	class PlayerAirSlashHigh : public PlayerAirState
	{
		/// @brief ジャンプの移動量カーブ
		AnimationCurve jumpMovementCurve;

		/// @brief ジャンプの移動量
		float maxJumpMovement;

		/// @brief 1フレーム前のジャンプの移動量
		float prevJumpMovement;
	public:
		PlayerAirSlashHigh();
		~PlayerAirSlashHigh() {}

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief 開始
		void OnStartBehavior();

		/// @brief 更新
		void UpdateBehavior() override;

		/// @brief アニメーション終了時の行動
		/// @param _fromAnimNodeName 遷移元のアニメーションノード名
		/// @param _toAnimNodeName 遷移先のアニメーションノード名
		void OnAnimationEnd(const std::string& _fromAnimNodeName,
			const std::string& _toAnimNodeName) override;

		/// @brief 空中の移動
		void MoveUpdate();

		void ImGuiDebug() override;
	};
}


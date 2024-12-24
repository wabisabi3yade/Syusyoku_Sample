#pragma once
#include "BossAttackState.h"

namespace HashiTaku
{
	/// @brief ボスのジャンプ攻撃
	class BossJumpAttack : public BossAttackState
	{
		/// @brief ジャンプ縦の距離カーブ
		AnimationCurve jumpHeightDisCurve;

		/// @brief  ジャンプ横速度係数の距離カーブ
		AnimationCurve horiSpeedCofficientCurve;

		/// @brief ボスからオフセット離れたところに合わせてプレイヤーへ落ちる（武器に合わせたり）
		// ボスの正面方向から見て
		float fallPosOffset;

		/// @brief 現在の移動速度
		DirectX::SimpleMath::Vector3 curMoveSpeed;

		/// @brief 横移動で最大の速度
		float maxHoriSpeed;

		/// @brief ジャンプの高さ
		float maxJumpHeight;

		/// @brief ジャンプ速度係数
		float horiSpeedCofficient;

		/// @brief 1フレーム前までの高さ
		float prevJumpHeight;
	public:
		BossJumpAttack();
		~BossJumpAttack() {}

		/// @brief デバッグ描画
		void DebugDisplay();

		json Save() override;
		void Load(const json& _data) override;
	private:
		void OnStartBehavior();
		void UpdateBehavior();

		/// @brief ジャンプの動き
		void JumpMove();

		/// @brief 横の動き
		void HorizonMove();

		/// @brief 縦の動き
		void VertivalMove();

		void ImGuiDebug() override;
	};
}
#pragma once
#include "BossGroundState.h"

namespace HashiTaku
{
	/// @brief ボス地上移動
	class BossGroundMove : public BossActState_Base
	{
		/// @brief  最大移動速度
		float maxSpeed;

		/// @brief 加速度
		float acceleration;

		/// @brief 現在速度
		float currentSpeed;

		/// @brief 回転速度
		float rotateSpeed;
	public:
		BossGroundMove();
		virtual ~BossGroundMove() {}

		json Save() override;
		void Load(const json& _data) override;

	protected:
		/// @brief 遷移切り替え
		void TransitionCheckUpdate() override;

		void ImGuiDebug() override;
	private:
		/// @brief 更新処理
		void UpdateBehavior() override;

		/// @brief 状態切り替え終了処理
		void OnEndBehavior() override;

		/// @brief 自身を回転する
		void Rotation();

		/// @brief 移動処理
		void Move();

		/// @brief 近接の距離になっているか確認
		/// @return 遷移するか？
		bool CheckNearTransition();
	};
}
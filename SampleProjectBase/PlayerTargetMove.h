#pragma once
#include "PlayerMoveState.h"

namespace HashiTaku
{
	class PlayerTargetMove : public PlayerMoveState
	{
		/// @brief 回転速度
		float rotateSpeed;

	public:
		PlayerTargetMove();
		~PlayerTargetMove() {}

		/// @brief セーブする
		/// @return セーブデータ
		json Save() override;

		/// @brief ロードする
		/// @param _data ロードするデータ 
		void Load(const json& _data) override;
	private:
		void UpdateBehavior() override;
		void OnEndBehavior() override;
		void TransitionCheckUpdate() override;

		/// @brief ブレンド割合をセット
		void ApplyBlendAnim();

		/// @brief ルートモーションをセット
		void ApplyRootMotion();

		void ImGuiDebug() override;
	};
}

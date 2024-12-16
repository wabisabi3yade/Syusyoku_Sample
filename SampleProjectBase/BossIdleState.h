#pragma once
#include "BossGroundState.h"

namespace HashiTaku
{
	/// @brief 待機状態(ステートとステートの間を繋げる役割)
	class BossIdleState : public BossActState_Base
	{
		/// @brief 次の状態
		BossState nextState;

		/// @brief 次の状態が決まったか？
		bool isDecideNextState;

#ifdef EDIT
		bool isIdleOnly{ false };
#endif EDIT

	public:
		BossIdleState();
		~BossIdleState() {}

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	private:
		/// @brief 開始処理
		void OnStartBehavior() override;
		/// @brief 更新
		void UpdateBehavior() override;
		/// @brief  遷移切り替え
		void TransitionCheckUpdate() override;

		void ImGuiDebug() override;
	};
}
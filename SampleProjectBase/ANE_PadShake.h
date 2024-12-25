#pragma once
#include "AnimationNotifyEvent.h"

namespace HashiTaku
{
	/// @brief パッド振動
	class ANE_PadShake : public AnimationNotifyEvent
	{
		/// @brief パッド振動力
		float padShakePower;

		/// @brief パッド振動時間
		float padShakeTime;
	public:
		ANE_PadShake();
		~ANE_PadShake() {}

		/// @brief クローン関数
		/// @return 生成した通知イベント
		std::unique_ptr<AnimationNotify_Base> Clone() override;

		json Save() override;
		void Load(const json& _data) override;

	private:
		void OnEvent() override;

		void ImGuiDebug() override;
	};
}



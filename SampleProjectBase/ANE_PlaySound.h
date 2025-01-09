#pragma once
#include "AnimationNotifyEvent.h"

namespace HashiTaku
{
	/// @brief 効果音を再生する
	class ANE_PlaySound : public AnimationNotifyEvent
	{
		PlaySoundParameter soundParameter;

	public:
		ANE_PlaySound();
		~ANE_PlaySound() {}

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


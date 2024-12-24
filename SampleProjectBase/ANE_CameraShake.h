#pragma once
#include "AnimationNotifyEvent.h"
#include "PerlinShakeParameter.h"

namespace HashiTaku
{
	/// @brief イベントでカメラ揺れをする
	class ANE_CameraShake : public AnimationNotifyEvent
	{
		/// @brief カメラを揺らすパラメータ
		PerlinShakeParameter shakeParameter;

	public:
		ANE_CameraShake();
		~ANE_CameraShake() {}

		/// @brief クローン関数
		/// @return 通知クラス
		std::unique_ptr<AnimationNotify_Base> Clone() override;

		json Save() override;
		void Load(const json& _data) override;

	private:
		/// @brief イベント処理
		void OnEvent()  override;

		void ImGuiDebug() override;
	};
}


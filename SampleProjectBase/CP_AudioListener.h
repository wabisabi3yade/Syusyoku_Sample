#pragma once
#include "Component.h"

namespace HashiTaku
{
	/// @brief 3D音響でのリスナー側を設定するコンポーネント
	class CP_AudioListener : public Component
	{
		/// @brief DirectXリスナーのインスタンス
		std::unique_ptr<DX::AudioListener> pDXAudioListener;

	public:
		CP_AudioListener();
		~CP_AudioListener() {}

		void Init() override;
		void OnDestroy() override;

	private:
		void LateUpdate() override;
	};
}



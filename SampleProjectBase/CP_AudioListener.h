#pragma once
#include "Component.h"

namespace HashiTaku
{
	/// @brief 3D�����ł̃��X�i�[����ݒ肷��R���|�[�l���g
	class CP_AudioListener : public Component
	{
		/// @brief DirectX���X�i�[�̃C���X�^���X
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



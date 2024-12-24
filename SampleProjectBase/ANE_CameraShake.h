#pragma once
#include "AnimationNotifyEvent.h"
#include "PerlinShakeParameter.h"

namespace HashiTaku
{
	/// @brief �C�x���g�ŃJ�����h�������
	class ANE_CameraShake : public AnimationNotifyEvent
	{
		/// @brief �J������h�炷�p�����[�^
		PerlinShakeParameter shakeParameter;

	public:
		ANE_CameraShake();
		~ANE_CameraShake() {}

		/// @brief �N���[���֐�
		/// @return �ʒm�N���X
		std::unique_ptr<AnimationNotify_Base> Clone() override;

		json Save() override;
		void Load(const json& _data) override;

	private:
		/// @brief �C�x���g����
		void OnEvent()  override;

		void ImGuiDebug() override;
	};
}


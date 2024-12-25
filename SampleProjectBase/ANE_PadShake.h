#pragma once
#include "AnimationNotifyEvent.h"

namespace HashiTaku
{
	/// @brief �p�b�h�U��
	class ANE_PadShake : public AnimationNotifyEvent
	{
		/// @brief �p�b�h�U����
		float padShakePower;

		/// @brief �p�b�h�U������
		float padShakeTime;
	public:
		ANE_PadShake();
		~ANE_PadShake() {}

		/// @brief �N���[���֐�
		/// @return ���������ʒm�C�x���g
		std::unique_ptr<AnimationNotify_Base> Clone() override;

		json Save() override;
		void Load(const json& _data) override;

	private:
		void OnEvent() override;

		void ImGuiDebug() override;
	};
}



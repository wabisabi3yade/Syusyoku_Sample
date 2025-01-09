#pragma once
#include "AnimationNotifyEvent.h"

namespace HashiTaku
{
	/// @brief ���ʉ����Đ�����
	class ANE_PlaySound : public AnimationNotifyEvent
	{
		PlaySoundParameter soundParameter;

	public:
		ANE_PlaySound();
		~ANE_PlaySound() {}

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


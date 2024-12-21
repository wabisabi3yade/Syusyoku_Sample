#pragma once
#include "AnimationNotifyEvent.h"

namespace HashiTaku
{
	class ANE_DebugLog : public AnimationNotifyEvent
	{
		/// @brief �\�����b�Z�[�W
		std::string displayMessage;

#ifdef EDIT
		/// @brief �ҏW�����ǂ����H
		bool isEditing{ false };
#endif // EDIT

	public:
		ANE_DebugLog();
		~ANE_DebugLog() {}

		// ���b�Z�[�W���Z�b�g
		void SetMessage(const std::string& _message);

		void ImGuiDebug() override;

		/// @brief �N���[���֐�
		/// @return ���������ʒm�C�x���g
		std::unique_ptr<AnimationNotify_Base> Clone() override;

		json Save() override;
		void Load(const json& _data) override;
	private:
		void OnEvent() override;
	};
}
#pragma once
#include "AnimationNotifyState.h"

namespace HashiTaku
{
	class ANS_DebugLog : public AnimationNotifyState
	{
		std::string message[3];

		u_int selectId;

		char inputString[IM_INPUT_BUF];

		bool isEditing[3] = { false };
	public:
		ANS_DebugLog();
		~ANS_DebugLog() {};

		/// @brief �N���[���֐�
		/// @return ���������ʒm�C�x���g
		std::unique_ptr<AnimationNotify_Base> Clone() override;

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	private:
		/// @brief �C�x���g�J�n����
		void Begin() override;

		/// @brief �C�x���g�Ԃ̍X�V����
		void Tick() override;

		/// @brief �C�x���g�I������
		void End() override;

		void ImGuiDebug() override;
	};
}
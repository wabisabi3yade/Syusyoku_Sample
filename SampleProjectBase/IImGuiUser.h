#pragma once

namespace HashiTaku
{
	/// @brief ImGui���g�p����C���^�[�t�F�[�X
	class IImGuiUser
	{
		static bool isDisplay;

	public:
		IImGuiUser() {}
		virtual ~IImGuiUser() {}

		/// @brief ImGui�֐����Ăяo��
		void ImGuiCall();

		// �\���ł��邩�擾
		static bool GetIsImGuiUse();

	protected:
		/// @brief ImGui�Őݒ���s��
		virtual void ImGuiSetting() = 0;
	};
}
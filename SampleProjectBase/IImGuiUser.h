#pragma once

namespace HashiTaku
{
	/// @brief ImGui���g�p����C���^�[�t�F�[�X
	class IImGuiUser
	{
	public:
		IImGuiUser() {}
		virtual ~IImGuiUser() {}

		/// @brief ImGui�֐����Ăяo��
		void ImGuiCall();
	protected:
		/// @brief ImGui�Őݒ���s��
		virtual void ImGuiSetting() = 0;
	};
}
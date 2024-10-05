#pragma once

namespace HashiTaku
{
	/// @brief ImGuiを使用するインターフェース
	class IImGuiUser
	{
	public:
		IImGuiUser() {}
		virtual ~IImGuiUser() {}

		/// @brief ImGui関数を呼び出す
		void ImGuiCall();
	protected:
		/// @brief ImGuiで設定を行う
		virtual void ImGuiSetting() = 0;
	};
}
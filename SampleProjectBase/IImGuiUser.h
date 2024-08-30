#pragma once

namespace HashiTaku
{
	/// @brief ImGuiを使用するインターフェース
	class IImGuiUser
	{
		static bool isDisplay;

	public:
		IImGuiUser() {}
		virtual ~IImGuiUser() {}

		/// @brief ImGui関数を呼び出す
		void ImGuiCall();

		// 表示できるか取得
		static bool GetIsImGuiUse();

	protected:
		/// @brief ImGuiで設定を行う
		virtual void ImGuiSetting() = 0;
	};
}
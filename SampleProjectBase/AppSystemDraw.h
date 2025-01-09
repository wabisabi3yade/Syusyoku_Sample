#pragma once

namespace HashiTaku
{
	class AssetDisplay;
	class DX11BulletPhisics;

	/// @brief ImGuiでシステム関連の描画を行う
	class AppSystemDraw : public Singleton_Base<AppSystemDraw>, public IImGuiUser
	{
		friend class Singleton_Base<AppSystemDraw>;

		/// @brief 可変フレームレートクラス
		VariableFrameRate* pFrameRate;

	public:
		void Init(VariableFrameRate& _variabeFrameRate);

	private:
		AppSystemDraw();
		~AppSystemDraw() {}
		void ImGuiDebug() override;
		void ImGuiRenderTarget();
	};
}


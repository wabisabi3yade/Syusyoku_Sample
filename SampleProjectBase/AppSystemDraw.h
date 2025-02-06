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

		void ImGuiSceneTimeScale();

		// 物理エンジン関係
		void ImGuiPhisics();

		// Vfx関係
		void ImGuiEffekseer();

		// レンダーターゲット関係
		void ImGuiRenderTarget();
	};
}


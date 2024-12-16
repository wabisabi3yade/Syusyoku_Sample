#pragma once
#include "RenderTarget.h"
#include "DepthStencil.h"

namespace HashiTaku
{
	/// @brief 影描画クラス
	class ShadowDrawer : public Singleton_Base<ShadowDrawer>
	{
		friend class Singleton_Base<ShadowDrawer>;

		/// @brief 深度を書き込むレンダーターゲット
		std::unique_ptr<RenderTarget> pRenderTarget;

		/// @brief 深度ステンシル
		std::unique_ptr<DepthStencil> pDepthStencil;
	public:
		/// @brief 影描画クラス初期化
		void Init();
	private:
		ShadowDrawer() {}
		~ShadowDrawer() {}
	};
}
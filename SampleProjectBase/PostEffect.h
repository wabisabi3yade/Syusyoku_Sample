#pragma once
#include "RenderTarget.h"

namespace HashiTaku
{
	/// @brief ポストエフェクトの基底クラス
	class PostEffect
	{
	protected:
		/// @brief 情報を書き込むレンダーターゲット
		std::unique_ptr<RenderTarget> pRenderTarget;

	public:
		PostEffect();
		virtual ~PostEffect() {}
	};
}



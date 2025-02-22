#pragma once
#include "RenderTarget.h"
namespace HashiTaku
{
	/// @brief レンダーターゲットの管理クラス
	class RenderTargetCollection : public Singleton_Base<RenderTargetCollection>
	{
		friend class Singleton_Base<RenderTargetCollection>;

	public:
		/// @brief レンダーターゲットの種類
		enum class RenderTargetType
		{
			SceneDraw,	// シーン描画
			Blur,	// ブラー
			Bloom,	// ブルーム
			MaxNum
		};

	private:
		/// @brief レンダーターゲットのリスト
		std::array<std::unique_ptr<RenderTarget>,
			static_cast<u_int>(RenderTargetType::MaxNum)> renderTargetList;

		/// @brief 描画クラス
		D3D11_Renderer* pRenderer;

		RenderTargetCollection();
		~RenderTargetCollection() {}
	public:
		/// @brief レンダーターゲットを描画先に設定
		/// @param _setRTType レンダーターゲットを設定
		void SetRenderTarget(RenderTargetType _setRTType);

		void GetRenderTarget(RenderTargetType _setRTType);
	};
}



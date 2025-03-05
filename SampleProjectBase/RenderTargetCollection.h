#pragma once
#include "RenderTarget.h"
namespace HashiTaku
{
	/// @brief レンダーターゲットの管理クラス
	class RenderTargetCollection
	{
	public:
		/// @brief レンダーターゲットの種類
		enum class RenderTargetType
		{
			SceneDraw,	// シーン描画
			MotionVector,	// 移動ベクトル
			MotionBlur,	// モーションブラー
			Luminance,	// ブルーム
			CombineBlur,	// ブラー組み合わせ
			MaxNum
		};

	private:
		/// @brief レンダーターゲットのリスト
		std::array<std::unique_ptr<RenderTarget>,
			static_cast<u_int>(RenderTargetType::MaxNum)> renderTargetList;

		/// @brief 描画クラス
		D3D11_Renderer* pRenderer;
	public:
		RenderTargetCollection(D3D11_Renderer& renderer);
		~RenderTargetCollection() {}

		/// @brief レンダーターゲットを描画先に設定
		/// @param _setRTType レンダーターゲットを設定
		/// @param Zバッファの書き込みをするか？
		void SetRenderTarget(RenderTargetType _setRTType, bool _useDepthStencil);

		/// @brief レンダーターゲットを取得する
		/// @param _setRTType レンダーターゲットの種類
		/// @return レンダーターゲット
		RenderTarget* GetRenderTarget(RenderTargetType _setRTType);

		/// @brief レンダーターゲットをクリアする
		void Clear();
	private:
		/// @brief レンダーターゲットを作成
		void CreateRenderTargets();
	};
}



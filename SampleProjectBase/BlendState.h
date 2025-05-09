#pragma once
#include "IGetRenderPara.h"

namespace HashiTaku
{
	// ブレンドステートをするためのブレンドオブジェクト
	class BlendState : public IGetRenderPara<ID3D11BlendState*>
	{
	public:
		enum class BlendStateType
		{
			None,	// なにもなし
			Alpha,	// 半透明
			Additive,	// 加算
			Multiply,	// 乗算
			MaxNum	// 最大数
		};

	private:
		/// @brief ブレンドステート
		Microsoft::WRL::ComPtr<ID3D11BlendState> pBlendState;

		/// @brief ブレンドステートの種類
		BlendStateType blendType;

	public:
		// 引数：デバイス
		BlendState() {};
		~BlendState() {};

		/// @brief 初期化
		/// @param _pDevice デバイス
		/// @param _blendType 作成するブレンドステートの種類
		/// @return 初期化成功したか？
		bool Init(ID3D11Device& _pDevice, BlendStateType _blendType);

		/// @brief ブレンドステートオブジェクトを渡す
		/// @return ブレンドステートオブジェクト
		ID3D11BlendState* GetResourceObject() override { return pBlendState.Get(); }

	private:
		/// @brief 半透明ブレンドアルファを作成
		/// @param _desc 設定
		void CreateAlphaBlendDesc(D3D11_BLEND_DESC& _desc);

		/// @brief 加算ブレンドアルファを作成
		/// @param _desc 設定
		void CreateAdditiveBlendDesc(D3D11_BLEND_DESC& _desc);

		/// @brief 乗算ブレンドアルファを作成
		/// @param _desc 設定
		void CreateMultiplyBlendDesc(D3D11_BLEND_DESC& _desc);
	};
}
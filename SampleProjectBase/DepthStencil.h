#pragma once
#include "RenderTarget.h"

namespace HashiTaku
{
	// 深度ステンシルクラス
	class DepthStencil : public D3DTexture_Base
	{
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;

	public:
		DepthStencil() {}
		~DepthStencil() {}

		/// @brief クリアする
		void ClearColor();

		/// @brief 作成する
		/// @param _width 横幅
		/// @param _height 縦幅
		/// @param _useStencil 
		/// @return 作成成功したか？
		bool Create(UINT _width, UINT _height, bool _useStencil);

		ID3D11DepthStencilView* GetView() const;

	private:
		bool CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData = nullptr) override;
	};
}

#pragma once
#include "Texture.h"

// 深度ステンシルクラス
class DepthStencil : public Texture
{
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;

public:
	DepthStencil() {}
	~DepthStencil() {}

	/// @brief クリアする
	void Clear();

	/// @brief 作成する
	/// @param _width 横幅
	/// @param _height 縦幅
	/// @param _useStencil 
	/// @return 作成成功したか？
	bool Create(UINT _width, UINT _height, bool _useStencil);

	ID3D11DepthStencilView* GetView() const;
};


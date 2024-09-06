#pragma once
#include "Texture.h"

/// @brief レンダーターゲットクラス
class RenderTarget :  public Texture
{
	/// @brief レンダーターゲットビュー
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRTV;

public:
	RenderTarget() {}
	~RenderTarget() {}

	void Clear();
	void Clear(DirectX::SimpleMath::Color color);
	bool Create(DXGI_FORMAT format, UINT width, UINT height);
	bool CreateFromScreen();
	ID3D11RenderTargetView* GetView() const;
};


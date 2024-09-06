#pragma once
#include "Texture.h"

/// @brief �����_�[�^�[�Q�b�g�N���X
class RenderTarget :  public Texture
{
	/// @brief �����_�[�^�[�Q�b�g�r���[
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


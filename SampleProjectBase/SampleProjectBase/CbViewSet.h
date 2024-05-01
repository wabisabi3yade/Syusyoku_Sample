#pragma once

// ビュー変換行列
struct CbView
{
	DirectX::XMFLOAT4X4 view = {};
};

// GPUに渡すビュー変換行列とバッファ
struct CbViewSet
{
	CbView data;

	ID3D11Buffer* pBuffer = nullptr;
};
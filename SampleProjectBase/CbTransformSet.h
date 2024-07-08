#pragma once

// GPUに渡す頂点データ等の情報
struct CbTransform
{
	DirectX::XMFLOAT4X4 transform = {};
};


// GPUに渡す座標データと定数バッファをセットにした構造体
struct CbTransformSet
{
	// 頂点座標などのデータ
	CbTransform data;
	// 定数バッファ
	ID3D11Buffer* pBuffer = nullptr;
};
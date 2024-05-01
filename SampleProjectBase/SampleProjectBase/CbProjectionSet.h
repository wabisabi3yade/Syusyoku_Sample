#pragma once

// 投影行列
struct CbProjection
{
	DirectX::XMFLOAT4X4  projection = {};
};

// GPUに渡すプロジェクション行列と定数バッファをセットにした構造体
struct CbProjectionSet
{
	// 頂点座標などのデータ
	CbProjection data;
	// 定数バッファ
	ID3D11Buffer* pBuffer = nullptr;
};
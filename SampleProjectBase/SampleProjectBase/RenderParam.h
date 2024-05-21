#pragma once
#include "CbTransformSet.h"
#include "CbProjectionSet.h"
#include "CbViewSet.h"

// 描画に必要なパラメータクラス
class RenderParam
{
public:
	struct ConstBuffer
	{

	};
private:
	// GPUに渡す定数バッファを初期化する
	bool InitConstBuffer(ID3D11Device* _device);

public:
	CbTransformSet cbTransformSet;	// GPUに渡す座標データ
	CbProjectionSet cbProjectionSet;	// GPUに渡す投影データ
	CbViewSet cbViewSet;	// GPUに渡すビュー変換データ

	RenderParam() {};
	~RenderParam();

	/// <summary>
	/// パラメータを初期化する(バッファのサイズ確保)
	/// </summary>
	/// <param name="_device">デバイス</param>
	/// <returns>問題なくできたかどうかフラグ</returns>
	bool Init(ID3D11Device* _device);
};


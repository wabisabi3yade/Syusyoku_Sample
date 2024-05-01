#pragma once

class RenderParam;

// 描画に必要なリソースを取得するインターフェース
class IGetRenderResource
{
public :
	// 描画の情報を取得
	virtual RenderParam& GetParameter() = 0;
	// デバイスを取得
	virtual ID3D11Device* GetDevice()const = 0;
	// デバイスコンテキストえお取得
	virtual ID3D11DeviceContext* GetDeviceContext() = 0;
};
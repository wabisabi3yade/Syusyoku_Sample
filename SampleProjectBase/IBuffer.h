#pragma once

// バッファのインターフェース
class IBuffer
{
public:

	/// @brief バッファを作成
	/// @param size サイズ
	/// @param pInitData  データ
	virtual bool CreateBuffer(UINT size, void* pInitData) = 0;

	/// @brief バッファを取得する
	/// @return バッファ
	virtual const ID3D11Buffer& GetBuffer()const = 0;
};


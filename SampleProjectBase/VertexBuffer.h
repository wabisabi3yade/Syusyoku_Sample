#pragma once
#include "IBuffer.h"

//頂点バッファクラス
class VertexBuffer : public IBuffer
{
	/// @brief 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer;

public:
	VertexBuffer() {}
	~VertexBuffer() {}
	
	/// @brief 頂点バッファを作成
	/// @param size サイズ
	/// @param pInitData 頂点データ
	/// @return 成功フラグ
	bool CreateBuffer(UINT size, void* pInitData) override;

	/// @brief 頂点バッファを返す
	/// @return 頂点バッファ
	const ID3D11Buffer& GetBuffer()const override;
};


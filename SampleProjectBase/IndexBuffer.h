#pragma once
#include "Buffer_Base.h"

// インデックスバッファ
class IndexBuffer : public Buffer_Base
{
public:
	IndexBuffer() {}
	~IndexBuffer() {}

	/// @brief インデックスバッファを作成
	/// @param _allSize 全体のサイズ
	/// @param _elementSize 要素のサイズ
	/// @param pInitData インデックスデータ
	/// @return 成功フラグ
	bool CreateBuffer(u_int _allSize, u_int _elementSize, void* pInitData)override;

	void SetGPU() const override;
};


#pragma once
#include "Buffer_Base.h"

namespace HashiTaku
{
	 /// @brief 頂点バッファクラス
	class VertexBuffer : public Buffer_Base
	{
	public:
		VertexBuffer() {}
		~VertexBuffer() {}

		/// @brief 頂点バッファを作成
		/// @param _allSize 全体のサイズ
		/// @param _elementSize 要素のサイズ
		/// @param pInitData 頂点データ
		/// @return 成功フラグ
		bool CreateBuffer(u_int _allSize, u_int _elementSize, void* pInitData) override;

		/// @brief GPUにバッファを送る
		void SetGPU() const override;
	};
}
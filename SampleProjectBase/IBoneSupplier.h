#pragma once
namespace HashiTaku
{
	// シェーダーに渡すボーン行列構造体
	struct BoneCombMtricies
	{
		static constexpr u_int MAX_BONEMTX{ 100 };	// シェーダーの渡すボーン行列の最大数
		DXSimp::Matrix matrix[MAX_BONEMTX];
	};


	/// @brief ボーン行列のバッファを供給するインターフェース
	class IBoneBufferSupplier
	{

	public:
		IBoneBufferSupplier() = default;
		virtual ~IBoneBufferSupplier() = default;

		/// @brief ボーン行列のバッファを取得する
		/// @return ボーン行列のバッファ(転置済み)
		virtual BoneCombMtricies* GetBoneBuffer() = 0;
	};
}



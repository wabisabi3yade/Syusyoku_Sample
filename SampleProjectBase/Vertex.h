#pragma once

namespace HashiTaku
{
	// ウェイトの最大数
	constexpr u_int MAX_WEIGHT_NUM(4);

	// 頂点データ
	struct Vertex
	{
		/// @brief 座標
		DXSimp::Vector3 position;

		// 色(RGBA)
		DXSimp::Color color;

		/// @brief UV座標（テクスチャ座標）
		DXSimp::Vector2 uv;

		/// @brief 頂点の法線ベクトル
		DXSimp::Vector3 normal;

		/// @brief 影響されるボーンのID
		int boneIndex[MAX_WEIGHT_NUM]{ -1,-1,-1,-1 };

		/// @brief ボーンの影響度
		float boneWeight[MAX_WEIGHT_NUM]{ 0.0f };
	};
}
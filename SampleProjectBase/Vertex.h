#pragma once

// ウェイトの最大数
constexpr u_int MAX_WEIGHT_NUM(4);

// 頂点データ
struct Vertex
{
	/// @brief 座標
	DirectX::SimpleMath::Vector3 position;

	// 色(RGBA)
	DirectX::SimpleMath::Color color;

	/// @brief UV座標（テクスチャ座標）
	DirectX::SimpleMath::Vector2 uv;

	/// @brief 頂点の法線ベクトル
	DirectX::SimpleMath::Vector3 normal;

	/// @brief 影響されるボーンのID
	int boneIndex[MAX_WEIGHT_NUM]{ 0 };

	/// @brief ボーンの影響度
	float boneWeight[MAX_WEIGHT_NUM]{ 0.0f };

	/// @brief ボーンの数
	u_int boneCnt{ 0 };
};


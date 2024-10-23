#pragma once
#include "SingleMesh.h"

/// @brief 2Dポリゴン
class Polygon2D : public SingleMesh
{
public:
	Polygon2D();
	~Polygon2D(){}

	/// @brief ポリゴン作成
	void MakePolygon(const DirectX::SimpleMath::Vector2& _centerPos, const  DirectX::SimpleMath::Vector2& _size, const DirectX::SimpleMath::Vector3& _angles);
private:
	void SetUV(const DirectX::SimpleMath::Vector2& _startUV, const DirectX::SimpleMath::Vector2& _endUV);
};


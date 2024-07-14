#pragma once
#include "SingleMesh.h"

/// @brief 平面メッシュ
class PlaneMesh : public SingleMesh
{

	/// @brief 平面メッシュを作成
	void MakePlane();
public:
	PlaneMesh();
	PlaneMesh(const PlaneMesh& _other);
	~PlaneMesh() {}
	PlaneMesh& operator=(const PlaneMesh& _other);

	void SetUV(const DirectX::SimpleMath::Vector2& _startUV, const DirectX::SimpleMath::Vector2& _endUV);

private:
	void Copy(const PlaneMesh& _other);
};


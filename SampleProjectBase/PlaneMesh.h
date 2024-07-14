#pragma once
#include "SingleMesh.h"

/// @brief ���ʃ��b�V��
class PlaneMesh : public SingleMesh
{

	/// @brief ���ʃ��b�V�����쐬
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


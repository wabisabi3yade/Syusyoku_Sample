#pragma once
#include "SingleMesh.h"

namespace HashiTaku
{
	/// @brief ���ʃ��b�V��
	class PlaneMesh : public SingleMesh
	{
		/// @brief ���ʃ��b�V�����쐬
		void MakePlane();
	public:
		PlaneMesh();
		~PlaneMesh() {}

		void SetUV(const DXSimp::Vector2& _startUV, const DXSimp::Vector2& _endUV);
	};
}
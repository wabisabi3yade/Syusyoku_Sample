#pragma once
#include "SingleMesh.h"

namespace HashiTaku
{
	/// @brief 平面メッシュ
	class PlaneMesh : public SingleMesh
	{
		/// @brief 平面メッシュを作成
		void MakePlane();
	public:
		PlaneMesh();
		~PlaneMesh() {}

		void SetUV(const DXSimp::Vector2& _startUV, const DXSimp::Vector2& _endUV);
	};
}
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

		/// @brief 頂点座標を設定
		/// @param _vertexPosList 頂点座標(左上、右上、左下、右下の順番)
		void SetVertexPos(const std::array<DXSimp::Vector3, 4>& _vertexPosList);

		/// @brief UV座標を設定
		/// @param _startUV 左上UV
		/// @param _endUV 右下UV
		void SetUV(const DXSimp::Vector2& _startUV, const DXSimp::Vector2& _endUV);
	};
}
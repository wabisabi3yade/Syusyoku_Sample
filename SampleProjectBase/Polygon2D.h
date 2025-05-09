#pragma once
#include "SingleMesh.h"

namespace HashiTaku
{
	/// @brief 2Dポリゴン
	class Polygon2D : public SingleMesh
	{
	public:
		Polygon2D();
		~Polygon2D() {}

		/// @brief ポリゴンを作成
		/// @param _centerPos 中心座標
		/// @param _size サイズ
		/// @param _angles 角度
		void MakePolygon(const DXSimp::Vector3& _centerPos, const  DXSimp::Vector2& _size, const DXSimp::Vector3& _angles);

		/// @brief UV座標を指定する
		/// @param _startUV 左上UV座標
		/// @param _endUV 右上UV座標
		void SetUV(const DXSimp::Vector2& _startUV, const DXSimp::Vector2& _endUV);
	};
}
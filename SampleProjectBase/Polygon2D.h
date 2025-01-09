#pragma once
#include "SingleMesh.h"

namespace HashiTaku
{
	/// @brief 2D�|���S��
	class Polygon2D : public SingleMesh
	{
	public:
		Polygon2D();
		~Polygon2D() {}

		/// @brief �|���S�����쐬
		/// @param _centerPos ���S���W
		/// @param _size �T�C�Y
		/// @param _angles �p�x
		void MakePolygon(const DXSimp::Vector3& _centerPos, const  DXSimp::Vector2& _size, const DXSimp::Vector3& _angles);

		/// @brief UV���W���w�肷��
		/// @param _startUV ����UV���W
		/// @param _endUV �E��UV���W
		void SetUV(const DXSimp::Vector2& _startUV, const DXSimp::Vector2& _endUV);
	};
}
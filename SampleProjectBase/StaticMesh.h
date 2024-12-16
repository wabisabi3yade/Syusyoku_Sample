#pragma once
#include "Mesh_Group.h"

namespace HashiTaku
{
	// スタティックメッシュ
	class StaticMesh : public Mesh_Group
	{
	public:
		StaticMesh() : Mesh_Group(MeshType::SM) {}
		~StaticMesh() {}
	};
}
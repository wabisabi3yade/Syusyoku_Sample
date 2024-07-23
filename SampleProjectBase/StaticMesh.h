#pragma once
#include "Mesh_Group.h"

// スタティックメッシュ
class StaticMesh : public Mesh_Group
{
public:
	StaticMesh() : Mesh_Group(MeshType::SM) {}
	~StaticMesh() {}
};

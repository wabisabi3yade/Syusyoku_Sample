#pragma once
#include "Mesh_Group.h"

namespace HashiTaku
{
	// �X�^�e�B�b�N���b�V��
	class StaticMesh : public Mesh_Group
	{
	public:
		StaticMesh() : Mesh_Group(MeshType::SM) {}
		~StaticMesh() {}
	};
}
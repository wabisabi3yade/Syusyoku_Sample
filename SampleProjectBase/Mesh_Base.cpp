#include "pch.h"
#include "Mesh_Base.h"

#include "Material.h"

void Mesh_Base::AddMesh(std::unique_ptr<Mesh> _pMesh)
{
	pMeshes.push_back(std::move(_pMesh));
}

void Mesh_Base::AddMaterial(Material& _pMaterial)
{
	pMaterials.push_back(&_pMaterial);
}

u_int Mesh_Base::GetMeshNum()
{
	return static_cast<u_int>(pMeshes.size());
}

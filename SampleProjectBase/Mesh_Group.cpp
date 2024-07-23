#include "pch.h"
#include "Mesh_Group.h"

#include "Material.h"

void Mesh_Group::AddMesh(std::unique_ptr<SingleMesh> _pMesh)
{
	pMeshes.push_back(std::move(_pMesh));
}

void Mesh_Group::AddMaterial(Material* _pMaterial)
{
	pMaterials.push_back(_pMaterial);
}

const SingleMesh* Mesh_Group::GetMesh(u_int _meshIdx) const
{
	// 指定したメッシュが範囲超えたら
	if (_meshIdx + 1 > static_cast<u_int>(pMeshes.size())) return nullptr;

	return pMeshes[_meshIdx].get();
}

u_int Mesh_Group::GetMeshNum()
{
	return static_cast<u_int>(pMeshes.size());
}

Material* Mesh_Group::GetMaterial(u_int _materialIdx)
{
	// 指定したメッシュが範囲超えたら
	if (_materialIdx + 1 > static_cast<u_int>(pMaterials.size())) return nullptr;

	return pMaterials[_materialIdx];
}

DirectX::SimpleMath::Vector3 Mesh_Group::GetCenterPosition() const
{
	return centerPos;
}

DirectX::SimpleMath::Vector3 Mesh_Group::GetSize() const
{
	return size;
}

Mesh_Group::MeshType Mesh_Group::GetType() const
{
	return meshType;
}

void Mesh_Group::SetCenterPosition(const DirectX::SimpleMath::Vector3& _centerPos)
{
	centerPos = _centerPos;
}

void Mesh_Group::SetSize(const DirectX::SimpleMath::Vector3& _size)
{
	// 0以下にならないよう制限
	size = Vec3::Max(_size, 0.0f);
}
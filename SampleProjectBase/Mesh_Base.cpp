#include "pch.h"
#include "Mesh_Base.h"

#include "Material.h"

void Mesh_Base::AddMesh(std::unique_ptr<SingleMesh> _pMesh)
{
	pMeshes.push_back(std::move(_pMesh));
}

void Mesh_Base::AddMaterial(Material& _pMaterial)
{
	pMaterials.push_back(&_pMaterial);
}

const SingleMesh* Mesh_Base::GetMesh(u_int _meshIdx) const
{
	// �w�肵�����b�V�����͈͒�������
	if (_meshIdx + 1 > static_cast<u_int>(pMeshes.size())) return nullptr;

	return pMeshes[_meshIdx].get();
}

u_int Mesh_Base::GetMeshNum()
{
	return static_cast<u_int>(pMeshes.size());
}

Material* Mesh_Base::GetMaterial(u_int _materialIdx)
{
	// �w�肵�����b�V�����͈͒�������
	if (_materialIdx + 1 > static_cast<u_int>(pMaterials.size())) return nullptr;

	return pMaterials[_materialIdx];
}

void Mesh_Base::SetSize(const DirectX::SimpleMath::Vector3& _size)
{
	// 0�ȉ��ɂȂ�Ȃ��悤����
	size = Vec3::Max(_size, 0.0f);
}

#include "pch.h"
#include "Mesh_Group.h"

#include "Material.h"

// シェーダー管理
#include "ShaderCollection.h"

void Mesh_Group::AddMesh(std::unique_ptr<SingleMesh> _pMesh)
{
	pMeshes.push_back(std::move(_pMesh));
}

void Mesh_Group::AddMaterial(Material* _pMaterial)
{
	pMaterials.push_back(_pMaterial);
}

SingleMesh* Mesh_Group::GetMesh(u_int _meshIdx) const
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

u_int Mesh_Group::GetMaterialNum()
{
	return static_cast<u_int>(pMaterials.size());
}

DirectX::SimpleMath::Vector3 Mesh_Group::GetCenterPosition() const
{
	return centerPos;
}

DirectX::SimpleMath::Vector3 Mesh_Group::GetSize() const
{
	return size;
}

float Mesh_Group::GetScaleTimes() const
{
	return scaleTimes;
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

void Mesh_Group::SetScaleTimes(float _scaleTimes)
{
	assert(_scaleTimes > 0.0f);
	scaleTimes = std::max(_scaleTimes, 0.0f);
}

void Mesh_Group::SetVertexShader(const std::string& _vsName)
{
	VertexShader* pVS = ShaderCollection::GetInstance()->GetVertexShader(_vsName);
	if (pVS == nullptr)
		return;

	// マテリアルすべてに反映させる
	for (auto m : pMaterials)
	{
		m->SetVertexShader(*pVS);
	}
}

void Mesh_Group::SetPixelShader(const std::string& _psName)
{
	PixelShader* pPS = ShaderCollection::GetInstance()->GetPixelShader(_psName);
	if (pPS == nullptr)
		return;

	// マテリアルすべてに反映させる
	for (auto m : pMaterials)
	{
		m->SetPixelShader(*pPS);
	}
}

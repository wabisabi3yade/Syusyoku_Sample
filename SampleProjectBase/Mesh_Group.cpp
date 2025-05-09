#include "pch.h"
#include "Mesh_Group.h"
#include "Material.h"
#include "ShaderCollection.h"
#include "AssetGetter.h"

namespace HashiTaku
{
	void Mesh_Group::AddMesh(std::unique_ptr<SingleMesh> _pMesh)
	{
		pMeshes.push_back(std::move(_pMesh));
	}

	void Mesh_Group::AddMaterial(Material* _pMaterial)
	{
		materials.push_back(_pMaterial);
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
		if (_materialIdx + 1 > static_cast<u_int>(materials.size())) return nullptr;

		return materials[_materialIdx];
	}

	VertexShader* Mesh_Group::GetVertexShader()
	{
		return &materials[0]->GetVertexShader();
	}

	u_int Mesh_Group::GetMaterialNum()
	{
		return static_cast<u_int>(materials.size());
	}

	DXSimp::Vector3 Mesh_Group::GetCenterPosition() const
	{
		return centerPos;
	}

	DXSimp::Vector3 Mesh_Group::GetSize() const
	{
		return size;
	}

	float Mesh_Group::GetLoadOffsetScale() const
	{
		return loadScale;
	}

	DXSimp::Vector3 Mesh_Group::GetLoadOffsetAngles() const
	{
		return loadOffsetAngles;
	}

	Mesh_Group::MeshType Mesh_Group::GetType() const
	{
		return meshType;
	}

	void Mesh_Group::SetCenterPosition(const DXSimp::Vector3& _centerPos)
	{
		centerPos = _centerPos;
	}

	void Mesh_Group::SetSize(const DXSimp::Vector3& _size)
	{
		// 0以下にならないよう制限
		size = Vec3::Max(_size, 0.0f);
	}

	void Mesh_Group::SetLoadOffsetScale(float _scaleTimes)
	{
		assert(_scaleTimes > 0.0f);
		loadScale = std::max(_scaleTimes, 0.0f);
	}

	void Mesh_Group::SetLoadFlipY(bool _isLoadFlipY)
	{
		isFlipY = _isLoadFlipY;

		if (_isLoadFlipY)
			loadOffsetAngles.y = Mathf::roundDeg * 0.5f;
	}

	void Mesh_Group::SetIsRightHand(bool _isRightHand)
	{
		isRightHand = _isRightHand;
	}

	void Mesh_Group::SetIsGetSize(bool _isGetSize)
	{
		isGetSize = _isGetSize;
	}

	void Mesh_Group::SetVertexShader(const std::string& _vsName)
	{
		VertexShader* pVS = ShaderCollection::GetInstance()->GetVertexShader(_vsName);
		if (pVS == nullptr)
			return;

		// マテリアルすべてに反映させる
		for (auto m : materials)
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
		for (auto m : materials)
		{
			m->SetPixelShader(*pPS);
		}
	}

	json Mesh_Group::Save()
	{
		auto data = AssetPath_Base::Save();

		data["loadScale"] = loadScale;
		data["flipY"] = isFlipY;
		data["getSize"] = isGetSize;
		data["rightHand"] = isRightHand;

		// マテリアルのDiffuseテクスチャの名前をセーブ
		for (auto& mat : materials)
		{
			std::string diffTexName = "null";
			if (Texture* pTex = mat->GetDiffuseTexture())
			{
				diffTexName = pTex->GetAssetName();
			}

			data["matDiffuse"].push_back(diffTexName);
		}

		return data;
	}

	void Mesh_Group::Load(const json& _data)
	{
		using namespace HashiTaku;

		//// マテリアルのテクスチャをロード
		//json matDatas;
		//bool isData = LoadJsonDataArray("matDiffuse", matDatas, _data);
		//if (isData)
		//{
		//	u_int idx = 0;
		//	for (auto& pMat : materials)
		//	{
		//		std::string diffuseTex = matDatas[idx];
		//		if (diffuseTex == "null") continue;

		//		if (Texture* pLoadTex = AssetGetter::GetAsset<Texture>(diffuseTex))
		//			pMat->SetDiffuseTexture(*pLoadTex);

		//		idx++;
		//	}
		//}	
	}
}
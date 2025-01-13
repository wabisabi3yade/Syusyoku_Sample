#include "pch.h"
#include "CP_MeshRenderer.h"

// �V�X�e��
#include "AssetGetter.h"
#include "InSceneSystemManager.h"
#include "ShaderCollection.h"

#include "Geometory.h"

namespace HashiTaku
{
	constexpr u_int TEX_DIFUSSE_SLOT(0);	// �f�B�t���[�Y�e�N�X�`���̃X���b�g
	constexpr u_int ANIMBONE_SLOT(1);	// �f�B�t���[�Y�e�N�X�`���̃X���b�g
	constexpr float ORIGIN_SCALE(0.15f);	// ���_�\���̃I�u�W�F�N�g�̃X�P�[��
	constexpr DXSimp::Color ORIGIN_COLOR(1.0f, 1.0f, 0.0f);	// ���_�\���̃I�u�W�F�N�g�̐F

	CP_MeshRenderer::CP_MeshRenderer()
		: pRenderMesh(nullptr), pShadowDrawer(nullptr), pBoneBuffer(nullptr), isOriginDisplay(false), isShadow(true)
	{
	}

	void CP_MeshRenderer::Init()
	{
		// �e�������݃N���X���擾
		pShadowDrawer = &InSceneSystemManager::GetInstance()->GetShadowDrawer();
		pShadowDrawer->AddDepthWriteRenderer(*this);

		// �{�[�������N���X���擾����
		pBoneBuffer = GetGameObject().GetComponent<IBoneBufferSupplier>();
	}

	void CP_MeshRenderer::OnDestroy()
	{
		// �폜
		pShadowDrawer->RemoveDepthWriteRenderer(*this);
	}

	void CP_MeshRenderer::Draw()
	{
		if (!IsCanDraw()) return;

		RenderParam& rendererParam = Direct3D11::GetInstance()->GetRenderer()->GetParameter();

		// WVP���߂�
		Transform& transform = GetTransform();
		auto& wvp = rendererParam.GetWVP();
		wvp.world = CalcLoadMtx() * transform.GetWorldMatrix();
		wvp.world = wvp.world.Transpose();

		// ���b�V���`��
		DrawMesh(wvp);

		// ���_�\��
		OriginDisplay();
	}

	void CP_MeshRenderer::SetRenderMesh(Mesh_Group& _renderMesh)
	{
		pRenderMesh = &_renderMesh;
	}

	void CP_MeshRenderer::SetRenderMesh(const std::string& _assetName)
	{
		pRenderMesh = AssetGetter::GetAsset<Mesh_Group>(_assetName);
	}

	void CP_MeshRenderer::SetVertexShader(const std::string& _vsName)
	{
		assert(pRenderMesh != nullptr);

		pRenderMesh->SetVertexShader(_vsName);
	}

	void CP_MeshRenderer::SetPixelShader(const std::string& _psName)
	{
		assert(pRenderMesh != nullptr);

		pRenderMesh->SetPixelShader(_psName);
	}


	Material* CP_MeshRenderer::GetMaterial(u_int _meshIdx)
	{
		assert(_meshIdx < pRenderMesh->GetMeshNum());

		u_int rendererMatCnt = static_cast<u_int>(setMaterials.size());
		if (_meshIdx < rendererMatCnt)	// �Z�b�g����Ă���}�e���A�����擾
		{
			Material* pSetMat = setMaterials[_meshIdx];
			if (pSetMat) return pSetMat;
		}

		// �܂��Z�b�g����Ă��Ȃ��Ȃ�
		// ���b�V��������擾
		u_int materialID = pRenderMesh->GetMesh(_meshIdx)->GetMaterialID();
		return  pRenderMesh->GetMaterial(materialID);
	}

	void CP_MeshRenderer::ImGuiDebug()
	{
#ifdef EDIT
		// ImGui�J�����Ƃ��������_�\��
		isOriginDisplay = true;

		// �e�`�悷�邩�H
		ImGui::Checkbox("IsShadow", &isShadow);

		// ���b�V���擾
		std::string meshName;
		if (pRenderMesh)
			meshName = pRenderMesh->GetAssetName();
		if (AssetGetter::ImGuiGetCombobox<Mesh_Group>("model", meshName))
		{
			pRenderMesh = AssetGetter::GetAsset<Mesh_Group>(meshName);
		}
		ImGuiMethod::LineSpaceSmall();

		ImGui::Text("Material Set");
		// �}�e���A�����擾
		u_int cnt = 0;
		auto endItr = setMaterials.end();
		for (auto matItr = setMaterials.begin(); matItr != endItr; ++matItr)
		{
			// �e�L�X�g
			std::string display = std::to_string(cnt) + ":";
			std::string matName = "Null";
			if (*matItr != nullptr)
				matName = (*matItr)->GetAssetName();

			display += matName;

			ImGui::Text(display.c_str());

			ImGui::SameLine();

			// Null�ɂ���
			if (ImGui::Button("Null"))
				*matItr = nullptr;

			cnt++;
		}
		std::string meshCntText = "meshCnt:" + static_cast<u_int>(setMaterials.size());
		ImGui::Text(meshCntText.c_str());
		if (ImGui::Button("+"))
		{
			setMaterials.push_back(nullptr);	// �g�ǉ�
		}
		ImGui::SameLine();
		if (ImGui::Button("-"))
		{
			setMaterials.pop_back();	// �g�ǉ�
		}


		ImGuiMethod::LineSpaceSmall();

		int useMaterialCnt = static_cast<int>(setMaterials.size());

		// �}�e���A�����w��
		ImGuiMethod::PushItemSmallWidth();
		static int inputId = 0;
		ImGui::DragInt("MatId", &inputId, 1, 0, useMaterialCnt - 1);
		ImGui::PopItemWidth();

		// �A�Z�b�g����}�e���A���I��
		static std::string getName;
		AssetGetter::ImGuiGetCombobox<Material>("##material", getName);

		// �Z�b�g����
		if (ImGui::Button("Set"))
		{
			if (inputId >= useMaterialCnt) return;

			// �}�e���A�����擾
			Material* pGetMat = AssetGetter::GetAsset<Material>(getName);
			if (!pGetMat) return;

			// �Z�b�g
			auto setItr = std::next(setMaterials.begin(), inputId);
			(*setItr) = pGetMat;
		}

#endif // EDIT
	}

	Mesh_Group* CP_MeshRenderer::GetRenderMesh()
	{
		return pRenderMesh;
	}

	void CP_MeshRenderer::WriteDepth()
	{
		if (!isShadow) return;
		if (!IsCanDraw()) return;

		// WVP���߂�
		Transform& transform = GetTransform();
		DXSimp::Matrix worldMtx = CalcLoadMtx() * transform.GetWorldMatrix();
		worldMtx = worldMtx.Transpose();

		// ���b�V���̐[�x����������ł���
		u_int meshCnt = pRenderMesh->GetMeshNum();
		for (u_int meshLoop = 0; meshLoop < meshCnt; meshLoop++)
		{
			VertexShader* pVS = &GetMaterial(meshLoop)->GetVertexShader();

			// �A�j���[�V�����𔽉f
			if (pBoneBuffer)
				pVS->UpdateSubResource(ANIMBONE_SLOT, pBoneBuffer->GetBoneBuffer());

			// ���C�g�s������[���h�s��Ƃ��ăZ�b�g����
			pShadowDrawer->SetWorldMatrix(worldMtx, pVS);
			const SingleMesh* pSingleMesh = pRenderMesh->GetMesh(meshLoop);
			CP_Renderer::DrawMesh(*pSingleMesh);
		}
	}

	json CP_MeshRenderer::Save()
	{
		auto data = CP_Renderer::Save();

		data["isShadow"] = isShadow;

		if (pRenderMesh)
			data["meshName"] = pRenderMesh->GetAssetName();

		json& materialDatas = data["materials"];
		for (auto& material : setMaterials)
		{
			std::string materialName = "Null";
			if (material)
				materialName = material->GetAssetName();

			materialDatas.push_back(materialName);
		}

		return data;
	}

	void CP_MeshRenderer::Load(const json& _data)
	{
		CP_Renderer::Load(_data);

		LoadJsonBoolean("isShadow", isShadow, _data);

		pRenderMesh = LoadJsonAsset<Mesh_Group>("meshName", _data);

		json materialDatas;
		if (LoadJsonDataArray("materials", materialDatas, _data))
		{
			u_int matCnt = static_cast<u_int>(materialDatas.size());
			setMaterials.resize(matCnt);
			for (u_int m_i = 0; m_i < matCnt; m_i++)
			{
				std::string matName = materialDatas[m_i];
				Material* pMat = AssetGetter::GetAsset<Material>(matName);
				setMaterials[m_i] = pMat;
			}
		}
	}

	bool CP_MeshRenderer::IsCanDraw()
	{
		if (pRenderMesh == nullptr) return false;

		return true;
	}

	DXSimp::Matrix CP_MeshRenderer::CalcLoadMtx()
	{
		using namespace DXSimp;

		float loadScale = pRenderMesh->GetLoadOffsetScale();
		Vector3 loadAngles = pRenderMesh->GetLoadOffsetAngles() * Mathf::degToRad;

		return Matrix::CreateScale(Vector3::One * loadScale) *
			Matrix::CreateFromYawPitchRoll(loadAngles.y, loadAngles.x, loadAngles.z);
	}

	void CP_MeshRenderer::DrawMesh(RenderParam::WVP& _wvp)
	{
		// ���b�V����
		u_int meshCnt = pRenderMesh->GetMeshNum();

		for (u_int meshLoop = 0; meshLoop < meshCnt; meshLoop++)
		{
			// ���b�V�����擾
			const SingleMesh* pSingleMesh = pRenderMesh->GetMesh(meshLoop);

			// ����`�悷��}�e���A��
			Material* pRenderMaterial = GetMaterial(meshLoop);

			// �}�e���A���̕`�揀��
			MaterialSetup(_wvp, pRenderMaterial);

			// ���b�V���`��
			CP_Renderer::DrawMesh(*pSingleMesh);
		}
	}

	void CP_MeshRenderer::MaterialSetup(RenderParam::WVP& _wvp, Material* _pMaterial)
	{
		if (_pMaterial == nullptr) return;

		VertexShader& pVS = _pMaterial->GetVertexShader();
		PixelShader& pPs = _pMaterial->GetPixelShader();

		// �V�F�[�_�[�Ƀo�b�t�@�𑗂�
		// (�����ł̓��C�g�A�J�������W�Ȃǂ�1���[�v��1�x��������Ȃ����̂͑���Ȃ�)
		ShaderSetup(pVS, _wvp, *_pMaterial);
		ShaderSetup(pPs, _wvp, *_pMaterial);

		// �e�N�X�`���𑗂�
		pPs.SetTexture(TEX_DIFUSSE_SLOT, _pMaterial->GetDiffuseTexture());

		// GPU�ɑ���
		pVS.SetGPU();
		pPs.SetGPU();
	}

	void CP_MeshRenderer::ShaderSetup(Shader& _shader, RenderParam::WVP& _wvp, Material& _material)
	{
		// �o�b�t�@�̎�ނ���X���b�g�̔ԍ��ɑ���
		u_int bufferNum = _shader.GetBufferNum();

		using enum Shader::BufferType;

		for (u_int bufLoop = 0; bufLoop < bufferNum; bufLoop++)
		{
			switch (_shader.GetBufferType(bufLoop))
			{
			case WVP:	// WVP�s��
				_shader.UpdateSubResource(bufLoop, &_wvp);
				break;

			case Material:	// �}�e���A��
				_shader.UpdateSubResource(bufLoop, &_material.GetMaterialParameter());
				break;

			case BoneMatricies:	// �{�[��
				if (pBoneBuffer)
					_shader.UpdateSubResource(bufLoop, pBoneBuffer->GetBoneBuffer());
				/*	else
					{
						_shader.UpdateSubResource(bufLoop, )

					}*/
				break;

			default:
				break;
			}
		}
	}

	DXSimp::Vector3 CP_MeshRenderer::WorldOffset(const DXSimp::Vector3& _offset)
	{
		DXSimp::Vector3 worldOffset = _offset.x * GetTransform().Right();
		worldOffset += _offset.y * GetTransform().Up();
		worldOffset += _offset.z * GetTransform().Forward();

		return worldOffset;
	}

	void CP_MeshRenderer::OriginDisplay()
	{
#ifdef EDIT
		if (!isOriginDisplay) return;

		Geometory::SetPosition(GetTransform().GetPosition());
		Geometory::SetScale(ORIGIN_SCALE * DXSimp::Vector3::One);
		Geometory::SetColor(ORIGIN_COLOR);
		Geometory::DrawCube();

		isOriginDisplay = false;
#endif // EDIT
	}
}
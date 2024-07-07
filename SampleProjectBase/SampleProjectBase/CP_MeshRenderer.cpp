#include "pch.h"
#include "CP_MeshRenderer.h"

// �V�X�e��
#include "InSceneSystemManager.h"
#include "ShaderCollection.h"

void CP_MeshRenderer::DrawSetup()
{
	// �����_���[�擾
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();
	// ���[���h�ϊ��s����擾
	DirectX::SimpleMath::Matrix worldMatrix = D3D11_Renderer::GetWorldMtx(GetTransform());

	// ���[���h�ϊ��s��̍��W�Ƀ��f���̍��W������
	RenderParam::WVP wvp = renderer.GetParameter().GetWVP();
	wvp.world = worldMatrix;

	// �V�F�[�_�[�̐ݒ�
	VertexShader& pVs = pMaterials->GetVertexShader();
	PixelShader& pPs = pMaterials->GetPixelShader();

	pVs.UpdateBuffer(0, &wvp);

	MaterialParameter& materialParam = pMaterials->GetMaterialParameter();
	pVs.UpdateBuffer(1, &materialParam);

	// �f�B���N�V�������C�g�̏����擾
	SceneLights& sceneLights = InSceneSystemManager::GetInstance()->GetSceneLights();
	DirectionLParameter dirLightParam = sceneLights.GetDirectionParameter();
	pVs.UpdateBuffer(2, &dirLightParam);

	pPs.UpdateBuffer(0, &materialParam);

	pVs.Bind();
	pPs.Bind();
}

void CP_MeshRenderer::Init()
{
	name = "Mesh_Renderer";

	// �f�t�H���g�ŃL���[�u�����Ă���
	pMeshes = ResourceCollection::GetInstance()->GetResource<Mesh>("SM_Cube");

	ResourceCollection* reCol = ResourceCollection::GetInstance();
	// ���ɂ��邩�m�F
	const std::string MATERIAL_NAME = "M_Unlit";

	if (!reCol->GetImpotred(MATERIAL_NAME))	// ����������
	{
		// �}�e���A���쐬
		std::unique_ptr<Material> makeMaterial = std::make_unique<Material>();
		// �V�F�[�_�[��ݒ�
		ShaderCollection* shCol = ShaderCollection::GetInstance();
		VertexShader* v = shCol->GetVertexShader(shCol->defaultVS);
		PixelShader* p = shCol->GetPixelShader(shCol->defaultPS);
		makeMaterial->SetVertexShader(v);
		makeMaterial->SetPixelShader(p);

		// �Ǘ��N���X�ɃZ�b�g
		pMaterials = reCol->SetResource(MATERIAL_NAME, std::move(makeMaterial));
	}
	else	// ����Ȃ�
	{
		// �}�e���A�����擾
		pMaterials = reCol->GetResource<Material>(MATERIAL_NAME);
	}
}

void CP_MeshRenderer::Draw()
{
	for (auto pMesh : pMeshes)
	{
		DrawSetup();

		pMesh->Draw();
	}	
}

void CP_MeshRenderer::ImGuiSetting()
{
	for()
	pMaterials->ImGuiSetting();
}

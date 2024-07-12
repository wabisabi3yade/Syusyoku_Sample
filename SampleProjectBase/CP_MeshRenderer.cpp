#include "pch.h"
#include "CP_MeshRenderer.h"

// �V�X�e��
#include "InSceneSystemManager.h"
#include "ShaderCollection.h"

const RenderParam::WVP CP_MeshRenderer::WVPSetup()
{
	// �����_���[�擾
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();
	// ���[���h�ϊ��s����擾
	DirectX::SimpleMath::Matrix worldMatrix = D3D11_Renderer::GetWorldMtx(GetTransform());

	// ���[���h�ϊ��s��̍��W�Ƀ��f���̍��W������
	RenderParam::WVP wvp = renderer.GetParameter().GetWVP();
	wvp.world = worldMatrix;

	return wvp;
}

//void CP_MeshRenderer::MeshDraw(u_int _meshIdx, RenderParam::WVP& _wvp)
//{
//	//RenderMesh& renderMesh = renderMeshes[_meshIdx];
//
//	//// �V�F�[�_�[�̐ݒ�
//	//Material& useMaterial = *pMaterials[renderMesh.materialID];
//	//VertexShader& pVs = useMaterial.GetVertexShader();
//	//PixelShader& pPs = useMaterial.GetPixelShader();
//
//	//pVs.UpdateBuffer(0, &_wvp);
//
//	//MaterialParameter& materialParam = useMaterial.GetMaterialParameter();
//	//pVs.UpdateBuffer(1, &materialParam);
//
//	//// �f�B���N�V�������C�g�̏����擾
//	//SceneLights& sceneLights = InSceneSystemManager::GetInstance()->GetSceneLights();
//	//DirectionLParameter dirLightParam = sceneLights.GetDirectionParameter();
//	//pVs.UpdateBuffer(2, &dirLightParam);
//
//	//pPs.UpdateBuffer(0, &materialParam);
//
//	//pVs.Bind();
//	//pPs.Bind();
//
//	//// �`��
//	//renderMesh.pMesh->Draw();
//}

void CP_MeshRenderer::Init()
{
	//name = "Mesh_Renderer";

	//// �f�t�H���g�̃��b�V�������Ă���
	//AssetCollection& resourceCollect = *AssetCollection::GetInstance();
	//RenderMesh renderMesh;
	//renderMesh.pMesh = resourceCollect.GetResource<Mesh>("SM_Cube");
	//renderMesh.materialID = 0;
	//renderMeshes.push_back(renderMesh);

	//// �}�e���A�������邩�m�F
	//const std::string MATERIAL_NAME = "M_Unlit";
	//if (!resourceCollect.GetImpotred(MATERIAL_NAME))	// ����������
	//{
	//	// �}�e���A���쐬
	//	std::unique_ptr<Material> makeMaterial = std::make_unique<Material>();

	//	// �V�F�[�_�[���Z�b�g
	//	ShaderCollection* shCol = ShaderCollection::GetInstance();
	//	VertexShader* defaultVS = shCol->GetVertexShader(shCol->defaultVS);
	//	PixelShader* defaultPS = shCol->GetPixelShader(shCol->defaultPS);
	//	makeMaterial->SetVertexShader(defaultVS);
	//	makeMaterial->SetPixelShader(defaultPS);

	//	// �Ǘ��N���X�ɃZ�b�g
	//	resourceCollect.SetResource(MATERIAL_NAME, std::move(makeMaterial));
	//}

	//// �}�e���A��
	//Material& defaultMaterial = *resourceCollect.GetResource<Material>(MATERIAL_NAME);
	//pMaterials.push_back(&defaultMaterial);
}

void CP_MeshRenderer::Draw()
{
	//// ���[���h�ϊ��s������߂�
	//RenderParam::WVP wvp = WVPSetup();

	//for (u_int meshIdx = 0; meshIdx < static_cast<u_int>(renderMeshes.size()); meshIdx++)
	//{
	//	assert(renderMeshes[meshIdx].pMesh != nullptr && "���b�V���A�}�e���A�����Ȃ�");

	//	// ���b�V���`��
	//	MeshDraw(meshIdx, wvp);
	//}
}
//
//void CP_MeshRenderer::SetModel(Model& _model)
//{
//
//}

void CP_MeshRenderer::SetRenderMesh(Mesh_Base& _renderMesh)
{
	pRenderMesh = &_renderMesh;
}

void CP_MeshRenderer::ImGuiSetting()
{

}

#include "pch.h"
#include "CP_MeshRenderer.h"

// �V�X�e��
#include "AssetSetter.h"
#include "AssetGetter.h"
#include "InSceneSystemManager.h"
#include "ShaderCollection.h"

// ���b�V��
#include "StaticMesh.h"
#include "SkeletalMesh.h"



void CP_MeshRenderer::Init()
{
	name = "Mesh_Renderer";

	// �f�t�H���g�̃��b�V�������Ă���
	pRenderMesh = AssetGetter::GetAsset<StaticMesh>("SM_Cube");
}

void CP_MeshRenderer::Draw()
{
	// ���b�V���`��
	DrawMesh(WVPSetup());
}

void CP_MeshRenderer::SetRenderMesh(Mesh_Base& _renderMesh)
{
	pRenderMesh = &_renderMesh;
}

void CP_MeshRenderer::ImGuiSetting()
{
}

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

void CP_MeshRenderer::DrawMesh(RenderParam::WVP _wvp)
{
	u_int meshNum = pRenderMesh->GetMeshNum();
	for (u_int meshLoop = 0; meshLoop < meshNum; meshLoop++)
	{
		// ���b�V�����擾
		const SingleMesh* pSingleMesh = pRenderMesh->GetMesh(meshLoop);

		// �g�p����}�e���A���擾
		u_int materialID = pSingleMesh->GetMaterialID();
		Material* pMaterial = pRenderMesh->GetMaterial(materialID);

		// �V�F�[�_�[�̐ݒ�
		VertexShader& pVs = pMaterial->GetVertexShader();
		PixelShader& pPs = pMaterial->GetPixelShader();

		pVs.UpdateBuffer(0, &_wvp);
		MaterialParameter& materialParam = pMaterial->GetMaterialParameter();

		pVs.UpdateBuffer(1, &materialParam);

		// �f�B���N�V�������C�g�̏����擾
		SceneLights& sceneLights = InSceneSystemManager::GetInstance()->GetSceneLights();
		DirectionLParameter dirLightParam = sceneLights.GetDirectionParameter();
		pVs.UpdateBuffer(2, &dirLightParam);

		pPs.UpdateBuffer(0, &materialParam);

		if(materialParam.isTextureEnable)
		pPs.SetTexture(0, &pMaterial->GetDiffuseTexture());

		pVs.Bind();
		pPs.Bind();

		CP_Renderer::DrawMesh(*pSingleMesh);
	}
}
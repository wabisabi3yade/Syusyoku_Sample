#include "pch.h"
#include "CP_MeshRenderer.h"

// �V�X�e��
#include "AssetGetter.h"
#include "InSceneSystemManager.h"
#include "ShaderCollection.h"

// ���b�V��
#include "StaticMesh.h"
#include "SkeletalMesh.h"

constexpr u_int TEX_DIFUSSE_SLOT(0);	// �f�B�t���[�Y�e�N�X�`���̃X���b�g

using namespace DirectX::SimpleMath;

void CP_MeshRenderer::Init()
{
	name = "Mesh_Renderer";

	// �f�t�H���g�̃��b�V�������Ă���
	pRenderMesh = AssetGetter::GetAsset<StaticMesh>("SM_Cube");
}

void CP_MeshRenderer::Draw()
{
	RenderParam& rendererParam = Direct3D11::GetInstance()->GetRenderer()->GetParameter();

	// ���b�V���`��
	float scaleTimes = pRenderMesh->GetScaleTimes();
	DrawMesh(rendererParam.GetWVP(GetTransform()));
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

void CP_MeshRenderer::ImGuiSetting()
{
}

Mesh_Group* CP_MeshRenderer::GetRenderMesh()
{
	return pRenderMesh;
}

void CP_MeshRenderer::DrawMesh(RenderParam::WVP _wvp)
{
	u_int meshNum = pRenderMesh->GetMeshNum();

	// �X�P�[���{���s���������
	Matrix scaleMtx = Matrix::CreateScale(pRenderMesh->GetScaleTimes());
	_wvp.world = _wvp.world * scaleMtx;

	for (u_int meshLoop = 0; meshLoop < meshNum; meshLoop++)
	{
		// ���b�V�����擾
		const SingleMesh* pSingleMesh = pRenderMesh->GetMesh(meshLoop);

		// �g�p����}�e���A���擾
		u_int materialID = pSingleMesh->GetMaterialID();

		// �}�e���A���̕`�揀��
		MaterialSetup(_wvp, materialID);

		CP_Renderer::DrawMesh(*pSingleMesh);
	}
}

void CP_MeshRenderer::MaterialSetup(RenderParam::WVP& _wvp, u_int _mtrlIdx)
{
	Material* pMaterial = pRenderMesh->GetMaterial(_mtrlIdx);
	if (pMaterial == nullptr) return;

	VertexShader& pVS = pMaterial->GetVertexShader();
	PixelShader& pPs = pMaterial->GetPixelShader();

	// �V�F�[�_�[�Ƀo�b�t�@�𑗂�
	// (�����ł̓��C�g�A�J�������W�Ȃǂ�1���[�v��1�x��������Ȃ����̂͑���Ȃ�)
	ShaderSetup(pVS, _wvp, *pMaterial);
	ShaderSetup(pPs, _wvp, *pMaterial);

	// �e�N�X�`���𑗂�
	pPs.SetTexture(TEX_DIFUSSE_SLOT, pMaterial->GetDiffuseTexture());

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

		default:
			break;
		}
	}
}

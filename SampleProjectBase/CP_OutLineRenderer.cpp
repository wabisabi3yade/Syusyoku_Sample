#include "pch.h"
#include "CP_OutLineRenderer.h"
#include "ShaderCollection.h"
#include "GameObject.h"
#include "CP_Animation.h"

namespace HashiTaku
{
	VertexShader* CP_OutLineRenderer::pOutLineVS = nullptr;
	VertexShader* CP_OutLineRenderer::pAnimationOutLineVS = nullptr;
	PixelShader* CP_OutLineRenderer::pOutLinePS = nullptr;

	constexpr auto VS_NAME = "VS_OutLine";
	constexpr auto ANIMVS_NAME = "VS_OutLineAnimation";
	constexpr auto PS_NAME = "PS_OutLine";

	CP_OutLineRenderer::CP_OutLineRenderer()
		: pUseVetrexShader(nullptr), pRenderMesh(nullptr), pBoneBuffer(nullptr)
	{
	}

	void CP_OutLineRenderer::Init()
	{
		GetShader();
	}

	json CP_OutLineRenderer::Save()
	{
		auto data = CP_Renderer::Save();

		SaveJsonVector4("color", vsLineParameter.lineColor, data);
		data["scale"] = vsLineParameter.lineScale;
		data["ditherPower"] = psLineParameter.ditherPower;
		data["ditherOffset"] = psLineParameter.ditherOffset;
		return data;
	}

	void CP_OutLineRenderer::Load(const json& _data)
	{
		CP_Renderer::Load(_data);

		LoadJsonColor("color", vsLineParameter.lineColor, _data);
		LoadJsonFloat("scale", vsLineParameter.lineScale, _data);
		LoadJsonFloat("ditherPower", psLineParameter.ditherPower, _data);
		LoadJsonFloat("ditherOffset", psLineParameter.ditherOffset, _data);
	}

	void CP_OutLineRenderer::Start()
	{
		GetRenderMesh();

		SetUseShader();
	}

	void CP_OutLineRenderer::Draw()
	{
#ifdef EDIT
		// �`�悷����̂��Ȃ��Ȃ�
		if (!pRenderMesh) return;
#endif // EDIT

		// �`�揀��
		DrawSetup();

		// �`�悷��
		u_int meshNum = pRenderMesh->GetMeshNum();
		for (u_int meshLoop = 0; meshLoop < meshNum; meshLoop++)
		{
			// ���b�V�����擾
			const SingleMesh* pSingleMesh = pRenderMesh->GetMesh(meshLoop);
			CP_Renderer::DrawMesh(*pSingleMesh);
		}

		auto pRenderer = Direct3D11::GetInstance()->GetRenderer();
		pRenderer->SetCullingMode(D3D11_CULL_BACK);
	}

	void CP_OutLineRenderer::DrawSetup()
	{
		auto pRenderer = Direct3D11::GetInstance()->GetRenderer();
		RenderParam& rendererParam = pRenderer->GetParameter();

		pRenderer->SetCullingMode(D3D11_CULL_FRONT);
		
		// �V�F�[�_�[�Ƀo�b�t�@�𑗂�
		// (�����ł̓��C�g�A�J�������W�Ȃǂ�1���[�v��1�x��������Ȃ����̂͑���Ȃ�)
		Transform& transform = GetTransform();
		auto& wvp = rendererParam.GetWVP();

		// ���[���h�s��쐬
		wvp.world = MakeLoadMatrix() * transform.GetWorldMatrix();
		wvp.world = wvp.world.Transpose();

		// �o�b�t�@�[�𑗂�
		pUseVetrexShader->UpdateSubResource(0, &wvp);
		pUseVetrexShader->UpdateSubResource(1, &vsLineParameter);

		pOutLinePS->UpdateSubResource(1, &psLineParameter);

		// �A�j���[�V�������s���Ă���Ȃ�ǉ��Ńo�b�t�@�𑗂�
		if (IsAnimation())
		{
			// �R���r�l�[�V�����s��
			pUseVetrexShader->UpdateSubResource(2,
				pBoneBuffer->GetBoneBuffer());
		}

		// GPU�ɑ���
		pUseVetrexShader->SetGPU();
		pOutLinePS->SetGPU();
	}

	void CP_OutLineRenderer::GetShader()
	{
		// �V�F�[�_�[���܂��Ȃ��Ȃ�
		if (pOutLineVS) return;

		ShaderCollection* pShCol = ShaderCollection::GetInstance();
		pOutLineVS = pShCol->GetVertexShader(VS_NAME);
		pAnimationOutLineVS = pShCol->GetVertexShader(ANIMVS_NAME);
		pOutLinePS = pShCol->GetPixelShader(PS_NAME);
	}

	void CP_OutLineRenderer::GetRenderMesh()
	{
		// ���b�V�����擾����
		CP_MeshRenderer* pMeshRender = GetGameObject().GetComponent<CP_MeshRenderer>();
		if (!pMeshRender)	// �Ȃ��Ȃ�
		{
			// �������Ȃ��悤�ɂ���
			SetEnable(false);
			return;
		}

		pRenderMesh = pMeshRender->GetRenderMesh();
		// ���b�V�������_���[���`���x������
		SetPriority(pMeshRender->GetPriority() + 1);
	}

	void CP_OutLineRenderer::SetUseShader()
	{
		// �A�j���[�V�����R���|�[�l���g������΁A�A�j���[�V�����p�̒��_�V�F�[�_�[�ɂ���
		pBoneBuffer = GetGameObject().GetComponent<CP_Animation>();
		if (pBoneBuffer)
			pUseVetrexShader = pAnimationOutLineVS;
		else
			pUseVetrexShader = pOutLineVS;
	}

	bool CP_OutLineRenderer::IsAnimation() const
	{
		return pBoneBuffer != nullptr;
	}

	DXSimp::Matrix CP_OutLineRenderer::MakeLoadMatrix()
	{
		float loadScale = pRenderMesh->GetLoadOffsetScale();
		DXSimp::Vector3 loadAngles = pRenderMesh->GetLoadOffsetAngles() * Mathf::degToRad;

		return DXSimp::Matrix::CreateScale(DXSimp::Vector3::One * loadScale) *
			DXSimp::Matrix::CreateFromYawPitchRoll(loadAngles.y, loadAngles.x, loadAngles.z);
	}

	void CP_OutLineRenderer::ImGuiDebug()
	{
		ImGui::ColorEdit4("Color", &vsLineParameter.lineColor.x);
		ImGui::DragFloat("Scale", &vsLineParameter.lineScale, 0.001f, 0.0f, 100.0f);
		ImGui::DragFloat("DitherPower", &psLineParameter.ditherPower, 0.001f, 0.0f, 100.0f);
		ImGui::DragFloat("DitherOffset", &psLineParameter.ditherOffset, 0.001f, 0.0f, 100.0f);
	}
}
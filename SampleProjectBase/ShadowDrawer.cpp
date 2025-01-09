#include "pch.h"
#include "ShadowDrawer.h"
#include "DirectionLight.h"
#include "ShaderCollection.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	// �[�x�}�b�v�̃e�N�X�`���T�C�Y
	constexpr u_int TEXTURE_WIDTH(1920);
	constexpr u_int TEXTURE_HEIGHT(1080);

	// �r���[�|�[�g�T�C�Y
	constexpr u_int RENDERTARGET_WIDTH(1920);
	constexpr u_int RENDERTARGET_HEIGHT(1080);

	// �[�x�������݂Ɏg�p����V�F�[�_�[��
	constexpr auto DEPTHWRITE_PSNAME("PS_DepthWrite");

	// �f�B���N�V�������C�g�̍��W�����߂�Ƃ��̒����_����J��������̋���
	constexpr float CAM_TARGET_DISTANCE(25.0f);

	// �f�B���N�V�������C�g�̍��W�����Ƃ߂�Ƃ��̃J�����̒����_����̋���
	constexpr float DIR_LIGHT_DISTACE(50.0f);

	ShadowDrawer::ShadowDrawer(const DirectionLight& _light) :
		pDepthWritePS(nullptr), pLight(nullptr)
	{
		// ���C�g�擾
		pLight = &_light;

		// �[�x�������݃e�N�X�`��
		pDepthStencil = std::make_unique<DepthStencil>();
		pDepthStencil->Create(TEXTURE_WIDTH, TEXTURE_HEIGHT, false);

		// �[�x�`�惌���_�[�^�[�Q�b�g
		pRenderTarget = std::make_unique<RenderTarget>();
		pRenderer = Direct3D11::GetInstance()->GetRenderer();
		pRenderTarget->Create(DXGI_FORMAT_R32G32B32A32_FLOAT,
			RENDERTARGET_WIDTH,
			RENDERTARGET_HEIGHT);

		// �V�F�[�_�[�擾
		auto* pShCol = ShaderCollection::GetInstance();

		pDepthWritePS = pShCol->GetPixelShader(DEPTHWRITE_PSNAME);
		if (!pDepthWritePS) HASHI_DEBUG_LOG("�s�N�Z���V�F�[�_�[�擾���s");
	}

	void ShadowDrawer::CreateDepthTexture()
	{
#ifdef EDIT
		if (!GetCanDepthWrite()) return;
#endif // EDIT

		// �����_�[�^�[�Q�b�g�Ɛ[�x�������݃e�N�X�`�����N���A
		pRenderTarget->Clear();
		pDepthStencil->ClearColor();

		// ���C�g���猩���s����쐬����
		CalcLightMatrix();

		// �����_�[�^�[�Q�b�g�؂�ւ���(���_�V�F�[�_�[�͂��ꂼ��̕`��Ŏg�p���Ă�����̂��g�p)
		pRenderer->SetRenderTerget(1, pRenderTarget.get(), pDepthStencil.get());
		pDepthWritePS->SetGPU();

		// �[�x��������ł���
		for (auto& write : depthWriters)
		{
			write->WriteDepth();
		}

		// �I������烌���_�[�^�[�Q�b�g�����ɖ߂�
		pRenderer->SetBaseRenderTarget();
	}

	void ShadowDrawer::CalcLightMatrix()
	{
		auto* pInSceneManager = InSceneSystemManager::GetInstance();
		if (pInSceneManager->GetIsNullCamera()) return;	// �J�������Z�b�g����Ă��Ȃ��Ȃ�

		// �f�B���N�V�������C�g�̍��W���J�������狁�߂�
		Transform& camTrans = pInSceneManager->GetMainCamera().GetTransform();
		DXSimp::Vector3 camTargetPos = camTrans.GetPosition() +
			camTrans.Forward() * CAM_TARGET_DISTANCE;

		// �f�B���N�V�������C�g�̍��W�����߂�
		DXSimp::Vector3 lightDirection = pLight->GetDirection();
		lightDirection.Normalize();
		DXSimp::Vector3 lightPos = camTargetPos - lightDirection * DIR_LIGHT_DISTACE;
		// ���C�g���猩���s����쐬����
		DXSimp::Matrix lightViewMtx = DirectX::XMMatrixLookAtLH(
			lightPos,
			lightPos + lightDirection,
			Vec3::Up
		);
		lightViewMtx.Transpose(lightMatrix.view);

		// �v���W�F�N�V����
		DXSimp::Matrix projtMtx = DirectX::XMMatrixOrthographicLH(
			50.0f,
			50.0f,
			0.1f,
			100.0f
		);

		projtMtx.Transpose(lightMatrix.projection);
	}

	bool ShadowDrawer::GetCanDepthWrite() const
	{
		if (!pDepthWritePS) return false;
		if (!pLight) return false;

		return true;
	}

	void ShadowDrawer::SetWorldMatrix(const DXSimp::Matrix& _worldMtx, VertexShader* _pVs)
	{
		lightMatrix.world = _worldMtx;

		_pVs->UpdateSubResource(0, &lightMatrix);
		_pVs->SetGPU();
	}

	RenderParam::WVP& ShadowDrawer::GetLightMatrix()
	{
		return lightMatrix;
	}

	RenderTarget& ShadowDrawer::GetDepthRTV()
	{
		return *pRenderTarget;
	}

	void ShadowDrawer::AddDepthWriteRenderer(IApplyShadowDepth& _renderer)
	{
		auto itr = std::find(depthWriters.begin(), depthWriters.end(), &_renderer);
		if (itr != depthWriters.end())
		{
			HASHI_DEBUG_LOG("���ɒǉ�����Ă��܂�");
			return;
		}

		depthWriters.push_back(&_renderer);
	}

	void ShadowDrawer::RemoveDepthWriteRenderer(IApplyShadowDepth& _renderer)
	{
		depthWriters.remove(&_renderer);
	}

	void ShadowDrawer::ImGuiDebug()
	{
#ifdef EDIT
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		ImGui::Image((void*)&pDepthStencil->GetSRV(), viewportSize);
#endif // EDIT
	}
}
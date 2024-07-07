#include "pch.h"
#include "CP_SpriteRenderer.h"
#include "ShaderCollection.h"

#include "Material.h"

#include "InSceneSystemManager.h"

void CP_SpriteRenderer::Copy(const CP_SpriteRenderer& _other)
{
}

void CP_SpriteRenderer::DrawSetup()
{
	// �����_���[�擾
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	// ���[���h�ϊ��s����擾
	DirectX::SimpleMath::Matrix worldMatrix = D3D11_Renderer::GetWorldMtx(GetTransform());

	// ���[���h�ϊ��s��̍��W�Ƀ��f���̍��W������
	RenderParam::WVP wvp = renderer.GetParameter().GetWVP();
	wvp.world = worldMatrix;

	// �V�F�[�_�[�̐ݒ�
	VertexShader& pVs = pMaterial->GetVertexShader();
	PixelShader& pPs = pMaterial->GetPixelShader();

	pVs.UpdateBuffer(0, &wvp);
	MaterialParameter materialParam = pMaterial->GetMaterialParameter();
	pVs.UpdateBuffer(1, &materialParam);

	// �f�B���N�V�������C�g�̏����擾
	SceneLights& sceneLights = InSceneSystemManager::GetInstance()->GetSceneLights();
	DirectionLParameter dirLightParam = sceneLights.GetDirectionParameter();
	pVs.UpdateBuffer(2, &dirLightParam);

	Texture& diffuseTex = pMaterial->GetDiffuseTexture();
	pPs.UpdateBuffer(0, &materialParam);
	pPs.SetTexture(0, &diffuseTex);

	pVs.Bind();
	pPs.Bind();
}

void CP_SpriteRenderer::MaterialSetup()
{
	// �f�t�H���g�Ń}�e���A����ݒ肷��
	const std::string MATERIAL_NAME = "M_SpriteUnlit";
	ResourceCollection* recol = ResourceCollection::GetInstance();
	if (recol->GetImpotred(MATERIAL_NAME))	// �f�t�H���g�̃}�e���A��������
	{
		pMaterial = recol->GetResource<Material>(MATERIAL_NAME);
	}
	else		// �Ȃ�
	{
		// �}�e���A�����쐬���A�Ǘ��N���X�ɃZ�b�g
		std::unique_ptr<Material> createMaterial = std::make_unique<Material>();

		ShaderCollection* shCol = ShaderCollection::GetInstance();
		VertexShader* pVs = shCol->GetVertexShader(shCol->defaultVS);
		PixelShader* pPs = shCol->GetPixelShader(shCol->defaultPS);
		createMaterial->SetVertexShader(pVs);
		createMaterial->SetPixelShader(pPs);

		pMaterial = ResourceCollection::GetInstance()->SetResource<Material>(MATERIAL_NAME, std::move(createMaterial));
	}
}

CP_SpriteRenderer& CP_SpriteRenderer::operator=(const CP_SpriteRenderer& _other)
{
	if (this == &_other) return *this;

	Copy(_other);

	return *this;
}

void CP_SpriteRenderer::Init()
{
	name = "SpriteRenderer";

	// �X�v���C�g�쐬
	pSprite = std::make_unique<Sprite>();

	// �}�e���A��������
	MaterialSetup();
}

void CP_SpriteRenderer::Draw()
{
	// �`�揀��
	DrawSetup();

	pSprite->Draw();
}

void CP_SpriteRenderer::SetTexture(Texture& _texture)
{
	// �}�e���A���ɓn��
	pMaterial->SetDiffuseTexture(_texture);

	// �X�v���C�g�ɓn��
	pSprite->SetTexture(_texture);
}

void CP_SpriteRenderer::SetMaterial(Material& _material)
{
	pMaterial = &_material;
}

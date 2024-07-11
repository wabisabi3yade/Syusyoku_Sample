#include "pch.h"
#include "CP_SpriteRenderer.h"
#include "ShaderCollection.h"

// �A�Z�b�g�֘A
#include "AssetSetter.h"
#include "AssetGetter.h"

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

	Texture& pTex = pSprite->GetTexture();
	pPs.UpdateBuffer(0, &materialParam);
	pPs.SetTexture(0, &pTex);

	pVs.Bind();
	pPs.Bind();
}

void CP_SpriteRenderer::MaterialSetup()
{
	// �f�t�H���g�Ń}�e���A����ݒ肷��
	const std::string MATERIAL_NAME = "M_SpriteUnlit";
	
	// ���ɍ쐬�ς݂Ȃ�
	if (AssetSetter::CheckImport<Material>(MATERIAL_NAME))
	{
		pMaterial = AssetGetter::GetAsset<Material>(MATERIAL_NAME);
		return;
	}

	// �}�e���A���쐬���A�A�Z�b�g���Z�b�g����
	std::unique_ptr<Material> pCreateMaterial = std::make_unique<Material>();
	pMaterial = AssetSetter::SetAsset(MATERIAL_NAME, std::move(pCreateMaterial));
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
}

void CP_SpriteRenderer::SetTexture(Texture& _texture)
{
	// �X�v���C�g�ɓn��
	pSprite->SetTexture(_texture);
}

void CP_SpriteRenderer::SetMaterial(Material& _material)
{
	pMaterial = &_material;
}

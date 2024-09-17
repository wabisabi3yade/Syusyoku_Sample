#include "pch.h"
#include "CP_SpriteRenderer.h"
#include "ShaderCollection.h"

// �A�Z�b�g�֘A
#include "AssetSetter.h"
#include "AssetGetter.h"

#include "InSceneSystemManager.h"

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


void CP_SpriteRenderer::DrawSetup()
{
	struct TexEnable
	{
		int isTexEnable;
		float dummy[3];
	};

	// �����_���[�擾
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	// ���[���h�ϊ��s��̍��W�Ƀ��f���̍��W������
	RenderParam::WVP wvp = renderer.GetParameter().GetWVP(GetTransform());
	wvp.world = wvp.world.Transpose();

	// �V�F�[�_�[�̐ݒ�
	VertexShader& pVs = pMaterial->GetVertexShader();
	PixelShader& pPs = pMaterial->GetPixelShader();

	MaterialParameter& materialParam = pMaterial->GetMaterialParameter();
	materialParam.isTextureEnable = pSprite->GetIsTexEnable();

	Texture* pTex = pSprite->GetTexture();

	pVs.UpdateSubResource(0, &wvp);
	pVs.UpdateSubResource(1, &materialParam);

	TexEnable texEnable;
	texEnable.isTexEnable = materialParam.isTextureEnable;
	pPs.UpdateSubResource(0, &texEnable);
	pPs.SetTexture(0, pTex);

	pVs.SetGPU();
	pPs.SetGPU();
}

CP_SpriteRenderer::CP_SpriteRenderer()
{

}

CP_SpriteRenderer::CP_SpriteRenderer(const CP_SpriteRenderer& _other)
{
	Copy(_other);
}

CP_SpriteRenderer& CP_SpriteRenderer::operator=(const CP_SpriteRenderer& _other)
{
	Copy(_other);

	return *this;
}

void CP_SpriteRenderer::Init()
{
	// �X�v���C�g�쐬
	pSprite = std::make_unique<Sprite>();

	// �}�e���A��������
	MaterialSetup();
}

void CP_SpriteRenderer::Draw()
{
	// �`�揀��
	DrawSetup();

	// �l�p�`�|���S����`��
	CP_Renderer::DrawMesh(pSprite->GetSquare());
}

void CP_SpriteRenderer::ImGuiSetting()
{
	const u_int Buf = 256;
	static char str[Buf];
	ImGui::InputText("texture", str, Buf);

	if (ImGui::Button("Set"))
	{
		Texture* pTex = AssetGetter::GetAsset<Texture>(str);

		if (pTex)
			pSprite->SetTexture(*pTex);
	}
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

nlohmann::json CP_SpriteRenderer::Save()
{
	auto data = CP_Renderer::Save();

	data["sprite"] = pSprite->Save();

	return data;
}

void CP_SpriteRenderer::Load(const nlohmann::json& _data)
{
	CP_Renderer::Load(_data);

	if (!HashiTaku::IsJsonContains(_data, "sprite")) return;

	pSprite->Load(_data["sprite"]);
}

void CP_SpriteRenderer::Copy(const CP_SpriteRenderer& _other)
{
	if (this == &_other) return;

	CP_Renderer::operator=(_other);

	if (_other.pSprite)
		pSprite = std::make_unique<Sprite>(*_other.pSprite);

	if (_other.pMaterial)
		pMaterial = _other.pMaterial;
}

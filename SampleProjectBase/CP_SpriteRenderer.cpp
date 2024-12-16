#include "pch.h"
#include "CP_SpriteRenderer.h"
#include "ShaderCollection.h"

// �A�Z�b�g�֘A
#include "AssetSetter.h"
#include "AssetGetter.h"

#include "InSceneSystemManager.h"

namespace HashiTaku
{
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
		pMaterial->SetPixelShader("PS_TexColor");
	}


	void CP_SpriteRenderer::DrawSetup()
	{
		struct TexParam
		{
			int isTexEnable;
			float alpha;
			float dummy[2];
		};

		// �����_���[�擾
		D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

		// ���[���h�ϊ��s��̍��W�Ƀ��f���̍��W������
		RenderParam::WVP wvp = renderer.GetParameter().GetWVP();
		wvp.world = GetTransform().GetWorldMatrix();
		wvp.world = wvp.world.Transpose();

		// �V�F�[�_�[�̐ݒ�
		VertexShader& pVs = pMaterial->GetVertexShader();
		PixelShader& pPs = pMaterial->GetPixelShader();

		MaterialParameter& materialParam = pMaterial->GetMaterialParameter();
		materialParam.isTextureEnable = pSprite->GetIsTexEnable();

		Texture* pTex = pSprite->GetTexture();

		pVs.UpdateSubResource(0, &wvp);
		pVs.UpdateSubResource(1, &materialParam);

		TexParam texEnable;
		texEnable.isTexEnable = materialParam.isTextureEnable;
		texEnable.alpha = alpha;
		pPs.UpdateSubResource(0, &texEnable);
		pPs.SetTexture(0, pTex);

		pVs.SetGPU();
		pPs.SetGPU();
	}

	CP_SpriteRenderer::CP_SpriteRenderer() : alpha(1.0f)
	{
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

	void CP_SpriteRenderer::ImGuiDebug()
	{
		ImGui::SliderFloat("Alpha", &alpha, 0.0f, 1.0f);

		std::string texName;
		if (pSprite->GetIsTexEnable())
			texName = pSprite->GetTexture()->GetAssetName();

		if (AssetGetter::ImGuiGetCombobox<Texture>("texture", texName))
		{
			Texture* pTex = AssetGetter::GetAsset<Texture>(texName);
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

	void CP_SpriteRenderer::SetAlpha(float _alpha)
	{
		alpha = std::clamp(alpha, 0.0f, 1.0f);
	}

	float CP_SpriteRenderer::GetAlpha() const
	{
		return alpha;
	}

	nlohmann::json CP_SpriteRenderer::Save()
	{
		auto data = CP_Renderer::Save();

		data["sprite"] = pSprite->Save();
		data["alpha"] = alpha;

		return data;
	}

	void CP_SpriteRenderer::Load(const nlohmann::json& _data)
	{
		CP_Renderer::Load(_data);

		LoadJsonFloat("alpha", alpha, _data);

		if (IsJsonContains(_data, "sprite"))
		{
			pSprite->Load(_data["sprite"]);
		}

	}
}
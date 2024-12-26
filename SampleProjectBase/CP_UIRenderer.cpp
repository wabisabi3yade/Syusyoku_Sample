#include "pch.h"
#include "CP_UIRenderer.h"
#include "ShaderCollection.h"
#include "GameObject.h"
#include "AssetGetter.h"

namespace HashiTaku
{
	VertexShader* CP_UIRenderer::pVertexShader = nullptr;
	PixelShader* CP_UIRenderer::pPixelShader = nullptr;
	bool CP_UIRenderer::isSetShader = false;

	constexpr auto VS_NAME("VS_UI");	// ���_�V�F�[�_�[��
	constexpr auto PS_NAME("PS_UI");	// �s�N�Z���V�F�[�_�[��

	CP_UIRenderer::CP_UIRenderer() : pTexture(nullptr), alpha(1.0f)
	{
		// 2D�|���S���쐬
		pPolygon = std::make_unique<Polygon2D>();
	}

	void CP_UIRenderer::Init()
	{
		CP_Renderer::Init();

		// ��������Ă��Ȃ��Ȃ�
		if (!isSetShader)
		{
			// �V�F�[�_�[���擾����
			ShaderCollection& shaderCol = *ShaderCollection::GetInstance();
			pVertexShader = shaderCol.GetVertexShader(VS_NAME);
			pPixelShader = shaderCol.GetPixelShader(PS_NAME);

			isSetShader = true;
		}

		// UI�ɕύX����
		gameObject->SetLayer(Layer::Type::UI);
	}

	void CP_UIRenderer::OnChangeScale()
	{
		ReCreatePolygon();
	}

	void CP_UIRenderer::OnChangePosition()
	{
		ReCreatePolygon();
	}

	void CP_UIRenderer::OnChangeRotation()
	{
		ReCreatePolygon();
	}

	void CP_UIRenderer::SetTexture(const Texture& _setTexture)
	{
		pTexture = &_setTexture;
	}

	void CP_UIRenderer::SetUV(const DXSimp::Vector2& _startUV, const DXSimp::Vector2& _endUV)
	{
		pPolygon->SetUV(_startUV, _endUV);
	}

	void CP_UIRenderer::SetAlpha(float _alpha)
	{
		alpha = std::clamp(_alpha, 0.0f, 1.0f);
	}

	float CP_UIRenderer::GetAlpha() const
	{
		return alpha;
	}

	json CP_UIRenderer::Save()
	{
		auto data = CP_Renderer::Save();

		data["alpha"] = alpha;

		if (pTexture)
			data["image"] = pTexture->GetAssetName();

		return data;
	}

	void CP_UIRenderer::Load(const json& _data)
	{
		CP_Renderer::Load(_data);

		ReCreatePolygon();

		LoadJsonFloat("alpha", alpha, _data);

		std::string texName;
		if (LoadJsonString("image", texName, _data))
		{
			pTexture = AssetGetter::GetAsset<Texture>(texName);
		}
	}

	void CP_UIRenderer::Draw()
	{
		// ����
		DrawSetup();

		// �|���S���`��
		DrawMesh(*pPolygon);
	}

	void CP_UIRenderer::DrawSetup()
	{
		// �e�N�X�`�����g�p���邩�V�F�[�_�[�ɓn���\����
		struct TexEnable
		{
			int isTexEnable = 0;
			float alpha = 1.0f;
			int dummy[2];
		};

		// �����_���[�擾
		D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

		// �V�F�[�_�[�̐ݒ�
		TexEnable texEnable;
		texEnable.isTexEnable = pTexture != nullptr;
		texEnable.alpha = alpha;

		// ���e
		DXSimp::Matrix projection = renderer.GetParameter().GetProjectionMatrix();

		pVertexShader->UpdateSubResource(0, &projection);
		pPixelShader->UpdateSubResource(0, &texEnable);
		pPixelShader->SetTexture(0, pTexture);
		pVertexShader->SetGPU();
		pPixelShader->SetGPU();
	}

	void CP_UIRenderer::ReCreatePolygon()
	{
		const Transform& t = GetTransform();

		DXSimp::Vector2 s = DXSimp::Vector2(t.GetScale().x, t.GetScale().y);
		pPolygon->MakePolygon(t.GetPosition(), s, t.GetEularAngles());
	}

	void CP_UIRenderer::ImGuiDebug()
	{
		ImGui::SliderFloat("Alpha", &alpha, 0.0f, 1.0f);

		std::string texName;
		if (pTexture)
			texName = pTexture->GetAssetName();

		if (AssetGetter::ImGuiGetCombobox<Texture>("Image", texName))
		{
			pTexture = AssetGetter::GetAsset<Texture>(texName);
		}
	}
}
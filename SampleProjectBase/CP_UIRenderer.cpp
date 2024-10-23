#include "pch.h"
#include "CP_UIRenderer.h"
#include "ShaderCollection.h"
#include "GameObject.h"
#include "AssetGetter.h"

VertexShader* CP_UIRenderer::pVertexShader = nullptr;
PixelShader* CP_UIRenderer::pPixelShader = nullptr;
bool CP_UIRenderer::isSetShader = false;

constexpr auto VS_NAME("VS_UI");	// ���_�V�F�[�_�[��
constexpr auto PS_NAME("PS_UI");	// �s�N�Z���V�F�[�_�[��

CP_UIRenderer::CP_UIRenderer() : pTexture(nullptr)
{
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

	// 2D�|���S���쐬
	pPolygon = std::make_unique<Polygon2D>();

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

nlohmann::json CP_UIRenderer::Save()
{
	return Component::Save();
}

void CP_UIRenderer::Load(const nlohmann::json& _data)
{
	ReCreatePolygon();
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
	using namespace DirectX::SimpleMath;

	// �e�N�X�`�����g�p���邩�V�F�[�_�[�ɓn���\����
	struct TexEnable
	{
		int isTexEnable = 0;
		int dummy[3];
	};

	// �����_���[�擾
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	// �V�F�[�_�[�̐ݒ�
	TexEnable texEnable;
	texEnable.isTexEnable = pTexture != nullptr;

	// ���e
	Matrix projection = renderer.GetParameter().GetProjectionMatrix();

	pVertexShader->UpdateSubResource(0, &projection);
	pPixelShader->UpdateSubResource(0, &texEnable);
	pPixelShader->SetTexture(0, pTexture);
	pVertexShader->SetGPU();
	pPixelShader->SetGPU();
}

void CP_UIRenderer::ReCreatePolygon()
{
	using namespace DirectX::SimpleMath;
	const Transform& t = GetTransform();

	Vector2 p = Vector2(t.GetPosition().x, t.GetPosition().y);
	Vector2 s = Vector2(t.GetScale().x, t.GetScale().y);
	pPolygon->MakePolygon(p, s, t.GetEularAngles());
}

void CP_UIRenderer::ImGuiSetting()
{
	std::string texName;
	if (pTexture)
		texName = pTexture->GetAssetName();

	if (AssetGetter::ImGuiGetCombobox<Texture>("Image", texName))
	{
		pTexture = AssetGetter::GetAsset<Texture>(texName);
	}
}

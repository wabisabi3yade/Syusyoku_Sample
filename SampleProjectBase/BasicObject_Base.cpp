#include "pch.h"
#include "BasicObject_Base.h"
#include "ShaderCollection.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"

using namespace DirectX::SimpleMath;

BasicObject_Base::BasicObject_Base() : pMaterial{ nullptr }
{
	// ��{�I�u�W�F�N�g�p�̃}�e���A�����쐬����
	ResourceCollection* reCol = ResourceCollection::GetInstance();

	// ���ɂ��邩�m�F
	const std::string MATERIAL_NAME = "M_BaseObject";

	// ����������
	if (!reCol->GetImpotred(MATERIAL_NAME))	
	{
		// �}�e���A���쐬
		std::unique_ptr<Material> makeMaterial = std::make_unique<Material>();
		// �V�F�[�_�[��ݒ�
		ShaderCollection* shCol = ShaderCollection::GetInstance();
		VertexShader* v = shCol->GetVertexShader("VS_BaseObject");
		PixelShader* p = shCol->GetPixelShader("PS_VertexColor");
		makeMaterial->SetVertexShader(v);
		makeMaterial ->SetPixelShader(p);

		// ���̃I�u�W�F�N�g�Ƀ}�e���A���Z�b�g
		pBaseMaterial = makeMaterial.get();	

		// �Ǘ��N���X�ɃZ�b�g
		reCol->SetResource(MATERIAL_NAME, std::move(makeMaterial));
	}
	else
	{
		// �}�e���A�����擾
		pBaseMaterial = reCol->GetResource<Material>(MATERIAL_NAME);	
	}
}

BasicObject_Base::~BasicObject_Base()
{
}

void BasicObject_Base::BasicDraw(Transform& _transform, DirectX::SimpleMath::Color& _color)
{
	D3D11_Renderer* renderer = Direct3D11::GetInstance()->GetRenderer();
	
	// ���[���h�ϊ��s����擾
	Matrix worldmtx = D3D11_Renderer::GetWorldMtx(_transform);
	
	// ���[���h�ϊ��s���WVP�ɓ����
	RenderParam::WVP wMat  = renderer->GetParameter().GetWVP();
	wMat.world = worldmtx;

	// �V�F�[�_�[���Z�b�g
	pBaseMaterial->GetVertexShader().Bind();
	pBaseMaterial->GetPixelShader().Bind();

	// �V�F�[�_�[�Ƀo�b�t�@�𑗂�
	pBaseMaterial->GetVertexShader().UpdateBuffer(0, &wMat);
	pBaseMaterial->GetVertexShader().UpdateBuffer(1, &_color);

	Mesh::Draw();	
}

void BasicObject_Base::SetMaterial(Material& _material)
{
	pMaterial = &_material;
}

void BasicObject_Base::ImGuiSetting()
{
	if (pMaterial == nullptr) return;

	if (ImGui::TreeNode("Material"))
	{


		ImGui::TreePop();
	}
}

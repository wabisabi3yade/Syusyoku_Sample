#include "pch.h"
#include "BasicObject_Base.h"
#include "ShaderCollection.h"

using namespace DirectX::SimpleMath;

BasicObject_Base::BasicObject_Base() : color(Vector4::One)
{
	// ��{�I�u�W�F�N�g�p�̃}�e���A�����쐬����
	ResourceCollection* reCol = ResourceCollection::GetInstance();
	// ���ɂ��邩�m�F
	const std::string MATERIAL_NAME = "M_BaseObject";

	if (!reCol->GetImpotred(MATERIAL_NAME))	// ����������
	{
		// �}�e���A���쐬
		std::unique_ptr<Material> makeMaterial = std::make_unique<Material>();
		// �V�F�[�_�[��ݒ�
		ShaderCollection* shCol = ShaderCollection::GetInstance();
		PixelShader* p = shCol->GetPixelShader("PS_VertexColor");
		makeMaterial->pPixelShader = p;

		material = makeMaterial.get();	// ���̃I�u�W�F�N�g�Ƀ}�e���A���Z�b�g
		// �Ǘ��N���X�ɃZ�b�g
		reCol->SetResource(MATERIAL_NAME, std::move(makeMaterial));
	}
	else
	{
		// �}�e���A�����擾
		material = reCol->GetResource<Material>(MATERIAL_NAME);	
	}
}

BasicObject_Base::~BasicObject_Base()
{
}

void BasicObject_Base::Draw()
{
	D3D11_Renderer* renderer = Direct3D11::GetInstance()->GetRenderer();
	
	// ���[���h�ϊ��s����擾
	Matrix worldmtx = D3D11_Renderer::GetWorldMtx(transform);
	
	// ���[���h�ϊ��s���WVP�ɓ����
	RenderParam::WVP wMat  = renderer->GetParameter().GetWVP();
	wMat.world = worldmtx;

	// �V�F�[�_�[�Ƀo�b�t�@�𑗂�
	material->pVertexShader->UpdateBuffer(0, &wMat);
	// �V�F�[�_�[���Z�b�g
	material->pVertexShader->Bind();
	material->pPixelShader->Bind();

	Mesh::Draw(*renderer);

	transform.ResetParameter();	// �`�悵�I�����烊�Z�b�g
	color = Vector4::One;	// ���F�ɖ߂�
	color.w = 0.f;
}

void BasicObject_Base::SetColor(const DirectX::SimpleMath::Color& _color)
{
	color = _color;
}

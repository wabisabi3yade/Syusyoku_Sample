#include "pch.h"
#include "BasicObject_Base.h"
#include "ShaderCollection.h"

using namespace DirectX::SimpleMath;

BasicObject_Base::BasicObject_Base()
{
	// ��{�I�u�W�F�N�g�p�̃}�e���A�����쐬����
	ResourceCollection* reCol = ResourceCollection::GetInstance();
	// ���ɂ��邩�m�F
	const std::string MATERIAL_NAME = "M_BaseObject";

	if (!reCol->GetImpotred(MATERIAL_NAME))	// ����������
	{
		// �}�e���A���쐬
		std::unique_ptr<MaterialClass> makeMaterial = std::make_unique<MaterialClass>();
		// �V�F�[�_�[��ݒ�
		ShaderCollection* shCol = ShaderCollection::GetInstance();
		VertexShader* v = shCol->GetVertexShader("VS_BaseObject");
		PixelShader* p = shCol->GetPixelShader("PS_VertexColor");
		makeMaterial->SetVertexShader(v);
		makeMaterial ->SetPixelShader(p);

		material = makeMaterial.get();	// ���̃I�u�W�F�N�g�Ƀ}�e���A���Z�b�g
		// �Ǘ��N���X�ɃZ�b�g
		reCol->SetResource(MATERIAL_NAME, std::move(makeMaterial));
	}
	else
	{
		// �}�e���A�����擾
		material = reCol->GetResource<MaterialClass>(MATERIAL_NAME);	
	}
}

BasicObject_Base::~BasicObject_Base()
{
}

void BasicObject_Base::Draw(Transform& _transform, DirectX::SimpleMath::Color& _color)
{
	D3D11_Renderer* renderer = Direct3D11::GetInstance()->GetRenderer();
	
	// ���[���h�ϊ��s����擾
	Matrix worldmtx = D3D11_Renderer::GetWorldMtx(_transform);
	
	// ���[���h�ϊ��s���WVP�ɓ����
	RenderParam::WVP wMat  = renderer->GetParameter().GetWVP();
	wMat.world = worldmtx;

	// �V�F�[�_�[�Ƀo�b�t�@�𑗂�
	material->GetVertexShader().UpdateBuffer(0, &wMat);
	material->GetVertexShader().UpdateBuffer(1, &_color);
	// �V�F�[�_�[���Z�b�g
	material->GetVertexShader().Bind();
	material->GetVertexShader().Bind();

	Mesh::Draw(*renderer);	
}
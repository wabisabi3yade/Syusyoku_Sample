#include "pch.h"
#include "Geometory.h"
#include "ShaderCollection.h"

#include "Material.h"

using namespace DirectX::SimpleMath;

Transform Geometory::transform = {};
Material* Geometory::pMaterial = nullptr;
Cube* Geometory::pCube = nullptr;
Sphere* Geometory::pSphere = nullptr;

std::unique_ptr<DebugCube> Geometory::pDebugCube = nullptr;

Color Geometory::color = Color(1.0f, 1.0f, 1.0f, 1.0f);

void Geometory::DrawSetup()
{
	// �����_���[�擾
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();
	// ���[���h�ϊ��s����擾
	DirectX::SimpleMath::Matrix worldMatrix = D3D11_Renderer::GetWorldMtx(transform);

	// ���[���h�ϊ��s��̍��W�Ƀ��f���̍��W������
	RenderParam::WVP wvp = renderer.GetParameter().GetWVP();
	wvp.world = worldMatrix;

	// �V�F�[�_�[�̐ݒ�
	VertexShader& pVs = pMaterial->GetVertexShader();
	PixelShader& pPs = pMaterial->GetPixelShader();

	pVs.UpdateBuffer(0, &wvp);
	pVs.UpdateBuffer(1, &color);

	pVs.Bind();
	pPs.Bind();
}

void Geometory::Init()
{
	//AssetCollection* pReCollect = AssetCollection::GetInstance();

	//// �}�e���A���쐬
	//const std::string MATERIAL_NAME = "M_BaseObject";
	//std::unique_ptr<Material> makeMaterial = std::make_unique<Material>();

	//// �V�F�[�_�[��������
	//ShaderCollection* shCol = ShaderCollection::GetInstance();
	//VertexShader* v = shCol->GetVertexShader("VS_BaseObject");
	//PixelShader* p = shCol->GetPixelShader("PS_VertexColor");
	//makeMaterial->SetVertexShader(v);
	//makeMaterial->SetPixelShader(p);

	//// ���̃I�u�W�F�N�g�Ƀ}�e���A���Z�b�g
	//pMaterial = AssetCollection::GetInstance()->SetResource<Material>(MATERIAL_NAME, std::move(makeMaterial));

	//// ���b�V�����쐬
	//std::unique_ptr<Cube> createCube = std::make_unique<Cube>();
	//std::unique_ptr<DebugCube> createDebugCube = std::make_unique<DebugCube>();
	//std::unique_ptr<Sphere> createSphere = std::make_unique<Sphere>();

	//// ���b�V�����Z�b�g����
	//pCube = static_cast<Cube*>(pReCollect->SetResource<Mesh>("SM_Cube", std::move(createCube)));
	//pSphere = static_cast<Sphere*>(pReCollect->SetResource<Mesh>("SM_Sphere", std::move(createSphere)));
}

void Geometory::Release()
{
}

void Geometory::DrawCube(bool _isWireFrame)
{
	//DrawSetup();

	//if (_isWireFrame)
	//	pDebugCube->Draw(transform, color);
	//else
	//	pCube->Draw();

	//// ���ɖ߂�
	//transform.ResetParameter();
	//color = Color(1, 1, 1, 1);
}

void Geometory::DrawSphere(bool _isWireFrame)
{
	//DrawSetup();

	//if (!_isWireFrame)
	//	pSphere->Draw();
	//else
	//	pSphere->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	//// ���ɖ߂�
	//transform.ResetParameter();
	//color = Color(1, 1, 1, 1);
}

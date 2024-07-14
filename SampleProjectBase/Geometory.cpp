#include "pch.h"
#include "Geometory.h"

// ���b�V��
#include "StaticMesh.h"
#include "Cube.h"
#include "DebugCube.h"
#include "Sphere.h"

// �V�F�[�_�[
#include "ShaderCollection.h"

// �A�Z�b�g
#include "AssetSetter.h"
#include "AssetGetter.h"

using namespace DirectX::SimpleMath;

Transform Geometory::transform = {};
Material* Geometory::pMaterial = nullptr;
StaticMesh* Geometory::pCube = nullptr;
StaticMesh* Geometory::pSphere = nullptr;
StaticMesh* Geometory::pDebugCube = nullptr;
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
	// �}�e���A�����쐬
	MakeMaterial();

	// �L���[�u���쐬
	MakeCube();
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

void Geometory::MakeMaterial()
{
	// �}�e���A���쐬
	const std::string MATERIAL_NAME = "M_Geometory";
	std::unique_ptr<Material> makeMaterial = std::make_unique<Material>("VS_BaseObject", "PS_VertexColor");

	// ���̃I�u�W�F�N�g�Ƀ}�e���A���Z�b�g
	pMaterial = AssetSetter::SetAsset<Material>(MATERIAL_NAME, std::move(makeMaterial));
}

void Geometory::MakeCube()
{
	// �L���[�u�̃V���O�����b�V�����쐬
	std::unique_ptr<Cube> pSingleCube = std::make_unique<Cube>();

	// �A�Z�b�g�Ǘ��ɑ��郁�b�V��
	std::unique_ptr<StaticMesh> pSendCube = std::make_unique<StaticMesh>();
	pSendCube->AddMesh(std::move(pSingleCube));

	Material* pUnlit = AssetGetter::GetAsset<Material>("M_Unlit");
	pSendCube->AddMaterial(*pUnlit);

	pCube = AssetSetter::SetAsset("SM_Cube", std::move(pSendCube));
}

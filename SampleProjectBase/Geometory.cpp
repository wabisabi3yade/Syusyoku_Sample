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
std::vector<StaticMesh*> Geometory::pGeometory = {};
Color Geometory::color = Color(1.0f, 1.0f, 1.0f, 1.0f);

void Geometory::DrawSetup()
{
	// �����_���[�擾
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	// ���[���h�ϊ��s��̍��W�Ƀ��f���̍��W������
	RenderParam::WVP wvp = renderer.GetParameter().GetWVP(transform);

	// �V�F�[�_�[�̐ݒ�
	VertexShader& pVs = pMaterial->GetVertexShader();
	PixelShader& pPs = pMaterial->GetPixelShader();

	pVs.UpdateBuffer(0, &wvp);
	pVs.UpdateBuffer(1, &color);

	pVs.Bind();
	pPs.Bind();
}

void Geometory::Draw(StaticMesh& _staticMesh)
{
	ID3D11DeviceContext* pDeviceContext =
		Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

	const SingleMesh& singleMesh = *_staticMesh.GetMesh(0);

	// �g�|���W�[�ݒ�
	pDeviceContext->IASetPrimitiveTopology(singleMesh.GetTopology());

	// �o�b�t�@��GPU�ɑ���
	singleMesh.GetVertexBuffer().SetGPU();
	singleMesh.GetIndexBuffer().SetGPU();

	pDeviceContext->DrawIndexed(
		singleMesh.GetIndexNum(),
		0,
		0
	);
}

void Geometory::Init()
{
	// �}�e���A�����쐬
	MakeMaterial();

	// �L���[�u���쐬
	MakeGeometory();
}

void Geometory::Release()
{
}

void Geometory::DrawCube(bool _isWireFrame)
{
	DrawSetup();

	int geoType = static_cast<int>(GeoType::Cube);

	if (_isWireFrame)
		geoType = static_cast<int>(GeoType::WireCube);
	
	Draw(*pGeometory[geoType]);
	
	// ���ɖ߂�
	transform.ResetParameter();
	color = Color(1, 1, 1, 1);
}

void Geometory::DrawSphere(bool _isWireFrame)
{
	DrawSetup();

	int geoType = static_cast<int>(GeoType::Sphere);	
	Draw(*pGeometory[geoType]);

	// ���ɖ߂�
	transform.ResetParameter();
	color = Color(1, 1, 1, 1);
}

void Geometory::MakeMaterial()
{
	// �}�e���A���쐬
	const std::string MATERIAL_NAME = "M_Geometory";
	std::unique_ptr<Material> makeMaterial = std::make_unique<Material>("VS_BaseObject", "PS_VertexColor");

	// ���̃I�u�W�F�N�g�Ƀ}�e���A���Z�b�g
	pMaterial = AssetSetter::SetAsset<Material>(MATERIAL_NAME, std::move(makeMaterial));
}

void Geometory::MakeGeometory()
{
	// �쐬���郁�b�V��
	std::vector<std::unique_ptr<SingleMesh>> pMeshes;

	pMeshes.push_back(std::make_unique<Cube>());
	pMeshes.push_back(std::make_unique<DebugCube>());
	pMeshes.push_back(std::make_unique<Sphere>());

	// ���O
	std::vector<std::string> names =
	{
		"SM_Cube",
		"SM_WireCube",
		"SM_Sphere"
	};

	Material* pUnlit = AssetGetter::GetAsset<Material>("M_Unlit");

	// �A�Z�b�g�Ǘ��ɃZ�b�g����
	for (u_int loop = 0; loop < static_cast<u_int>(pMeshes.size()); loop++)
	{
		std::unique_ptr<StaticMesh> pSM = std::make_unique<StaticMesh>();

		// ���b�V���ƃ}�e���A�����Z�b�g
		pSM->AddMesh(std::move(pMeshes[loop]));
		pSM->AddMaterial(pUnlit);

		// �A�Z�b�g�Ǘ��ɃZ�b�g
		pGeometory.push_back(AssetSetter::SetAsset(names[loop], std::move(pSM)));
	}
}
#include "pch.h"
#include "Geometory.h"

// メッシュ
#include "StaticMesh.h"
#include "Cube.h"
#include "DebugCube.h"
#include "Sphere.h"

// シェーダー
#include "ShaderCollection.h"

// アセット
#include "AssetSetter.h"
#include "AssetGetter.h"

using namespace DirectX::SimpleMath;

Material* Geometory::pMaterial = nullptr;
std::vector<StaticMesh*> Geometory::pGeometory = {};
Vector3 Geometory::position = Vector3::Zero;
Vector3 Geometory::scale = Vector3::One;
Vector3 Geometory::eularAngle = Vector3::Zero;
Color Geometory::color = Color(1.0f, 1.0f, 1.0f, 1.0f);

void Geometory::DrawSetup()
{
	// レンダラー取得
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	// ワールド変換行列の座標にモデルの座標を入れる
	Quaternion q = Quat::ToQuaternion(eularAngle);
	RenderParam::WVP wvp = renderer.GetParameter().GetWVP(position, scale, q);

	// シェーダーの設定
	VertexShader& pVs = pMaterial->GetVertexShader();
	PixelShader& pPs = pMaterial->GetPixelShader();

	pVs.UpdateSubResource(0, &wvp);
	pVs.UpdateSubResource(1, &color);

	pVs.SetGPU();
	pPs.SetGPU();
}

void Geometory::Draw(StaticMesh& _staticMesh)
{
	ID3D11DeviceContext* pDeviceContext =
		Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

	const SingleMesh& singleMesh = *_staticMesh.GetMesh(0);

	// トポロジー設定
	pDeviceContext->IASetPrimitiveTopology(singleMesh.GetTopology());

	// バッファをGPUに送る
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
	// マテリアルを作成
	MakeMaterial();

	// キューブを作成
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

	// 元に戻す
	ResetParameter();
}

void Geometory::DrawSphere(bool _isWireFrame)
{
	DrawSetup();

	int geoType = static_cast<int>(GeoType::Sphere);
	Draw(*pGeometory[geoType]);

	// 元に戻す
	ResetParameter();
}

void Geometory::DrawLine(const DirectX::SimpleMath::Vector3& _start, const DirectX::SimpleMath::Vector3& _end)
{
	DrawSetup();

	ID3D11DeviceContext* pDeviceContext =
		Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

	// トポロジー設定
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void Geometory::MakeMaterial()
{
	// マテリアル作成
	const std::string MATERIAL_NAME = "M_Geometory";
	std::unique_ptr<Material> makeMaterial = std::make_unique<Material>("VS_Primitive", "PS_Primitive");
	makeMaterial->SetIsntSave();

	// このオブジェクトにマテリアルセット
	pMaterial = AssetSetter::SetAsset<Material>(MATERIAL_NAME, std::move(makeMaterial));
}

void Geometory::MakeGeometory()
{
	// 作成するメッシュ
	std::vector<std::unique_ptr<SingleMesh>> pMeshes;

	pMeshes.push_back(std::make_unique<Cube>());
	pMeshes.push_back(std::make_unique<DebugCube>());
	pMeshes.push_back(std::make_unique<Sphere>());

	// 名前
	std::vector<std::string> names =
	{
		"SM_Cube",
		"SM_WireCube",
		"SM_Sphere"
	};

	Material* pUnlit = AssetGetter::GetAsset<Material>("M_Unlit");

	// アセット管理にセットする
	for (u_int loop = 0; loop < static_cast<u_int>(pMeshes.size()); loop++)
	{
		std::unique_ptr<StaticMesh> pSM = std::make_unique<StaticMesh>();

		// メッシュとマテリアルをセット
		pSM->AddMesh(std::move(pMeshes[loop]));
		pSM->AddMaterial(pUnlit);
		pSM->SetIsntSave();

		// アセット管理にセット
		pGeometory.push_back(AssetSetter::SetAsset(names[loop], std::move(pSM)));
	}
}

void Geometory::ResetParameter()
{
	position = Vector3::Zero;
	scale = Vector3::One;
	eularAngle = Vector3::Zero;
	color = Color(1.f, 1.f, 1.f, 1.f);
}

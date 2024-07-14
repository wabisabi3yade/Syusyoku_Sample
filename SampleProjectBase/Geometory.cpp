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

Transform Geometory::transform = {};
Material* Geometory::pMaterial = nullptr;
StaticMesh* Geometory::pCube = nullptr;
StaticMesh* Geometory::pSphere = nullptr;
StaticMesh* Geometory::pDebugCube = nullptr;
Color Geometory::color = Color(1.0f, 1.0f, 1.0f, 1.0f);

void Geometory::DrawSetup()
{
	// レンダラー取得
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();
	// ワールド変換行列を取得
	DirectX::SimpleMath::Matrix worldMatrix = D3D11_Renderer::GetWorldMtx(transform);

	// ワールド変換行列の座標にモデルの座標を入れる
	RenderParam::WVP wvp = renderer.GetParameter().GetWVP();
	wvp.world = worldMatrix;

	// シェーダーの設定
	VertexShader& pVs = pMaterial->GetVertexShader();
	PixelShader& pPs = pMaterial->GetPixelShader();

	pVs.UpdateBuffer(0, &wvp);
	pVs.UpdateBuffer(1, &color);

	pVs.Bind();
	pPs.Bind();
}

void Geometory::Init()
{
	// マテリアルを作成
	MakeMaterial();

	// キューブを作成
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

	//// 元に戻す
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

	//// 元に戻す
	//transform.ResetParameter();
	//color = Color(1, 1, 1, 1);
}

void Geometory::MakeMaterial()
{
	// マテリアル作成
	const std::string MATERIAL_NAME = "M_Geometory";
	std::unique_ptr<Material> makeMaterial = std::make_unique<Material>("VS_BaseObject", "PS_VertexColor");

	// このオブジェクトにマテリアルセット
	pMaterial = AssetSetter::SetAsset<Material>(MATERIAL_NAME, std::move(makeMaterial));
}

void Geometory::MakeCube()
{
	// キューブのシングルメッシュを作成
	std::unique_ptr<Cube> pSingleCube = std::make_unique<Cube>();

	// アセット管理に送るメッシュ
	std::unique_ptr<StaticMesh> pSendCube = std::make_unique<StaticMesh>();
	pSendCube->AddMesh(std::move(pSingleCube));

	Material* pUnlit = AssetGetter::GetAsset<Material>("M_Unlit");
	pSendCube->AddMaterial(*pUnlit);

	pCube = AssetSetter::SetAsset("SM_Cube", std::move(pSendCube));
}

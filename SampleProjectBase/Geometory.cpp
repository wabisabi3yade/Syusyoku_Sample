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
	//AssetCollection* pReCollect = AssetCollection::GetInstance();

	//// マテリアル作成
	//const std::string MATERIAL_NAME = "M_BaseObject";
	//std::unique_ptr<Material> makeMaterial = std::make_unique<Material>();

	//// シェーダーを初期化
	//ShaderCollection* shCol = ShaderCollection::GetInstance();
	//VertexShader* v = shCol->GetVertexShader("VS_BaseObject");
	//PixelShader* p = shCol->GetPixelShader("PS_VertexColor");
	//makeMaterial->SetVertexShader(v);
	//makeMaterial->SetPixelShader(p);

	//// このオブジェクトにマテリアルセット
	//pMaterial = AssetCollection::GetInstance()->SetResource<Material>(MATERIAL_NAME, std::move(makeMaterial));

	//// メッシュを作成
	//std::unique_ptr<Cube> createCube = std::make_unique<Cube>();
	//std::unique_ptr<DebugCube> createDebugCube = std::make_unique<DebugCube>();
	//std::unique_ptr<Sphere> createSphere = std::make_unique<Sphere>();

	//// メッシュをセットする
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

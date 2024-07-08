#include "pch.h"
#include "Object_UI.h"
#include "ShaderCollection.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Material* Object_UI::pShareMaterial = nullptr;

void Object_UI::MakePoligon()
{
	// テクスチャをはるポリゴンを生成する(TRIANGLESTRIP)
	// 頂点座標
	constexpr int vertexNum = 4;
	Vertex* pVerticies = new Vertex[vertexNum];
	pVerticies[0].position = Vector3(100, 100, 0);
	pVerticies[1].position = Vector3(200, 100, 0);
	pVerticies[2].position = Vector3(100, 300, 0);
	pVerticies[3].position = Vector3(200, 300, 0);

	// uv座標
	pVerticies[0].uv = { 0.0f, 0.0f };	// 左上
	pVerticies[1].uv = { 1.0f / 3.0f, 0.0f };	// 右上
	pVerticies[2].uv = { 0.0f, 1.0f / 4.0f };	// 左下
	pVerticies[3].uv = { 1.0f / 3.0f, 1.0f / 4.0f };	// 右下

	// インデックス
	constexpr int indexNum = 4;
	u_int* pIndicies = new u_int[indexNum];
	pIndicies[0] = 0;
	pIndicies[1] = 1;
	pIndicies[2] = 2;
	pIndicies[3] = 3;

	// 色、頂点
	for (int i = 0; i < vertexNum; i++)
	{
		pVerticies[i].color = Vector4::One;	// 色
		pVerticies[i].normal = Vector3(0.0f, 0.0f, -1.0f);
	}

	// 四角ポリゴン
	poligon = std::make_unique<Mesh>();	// メッシュ作成
	poligon->SetVertexData(pVerticies, vertexNum, pIndicies, indexNum);

}

Object_UI::Object_UI() : pTexture(nullptr)
{
	MakePoligon();

	if (pShareMaterial == nullptr)	// 共通にマテリアルが設定されていないなら
	{
		MakeMaterial();	// マテリアルを作成する
	}
	
	layer.SetType(Layer::Type::UI);	// UIレイヤーに変える
}

Object_UI::~Object_UI()
{
}

void Object_UI::MakeMaterial()
{
	// マテリアルを生成
	std::unique_ptr<Material> pMakeMaterial = std::make_unique<Material>();

	// シェーダーを設定
	ShaderCollection* shCol = ShaderCollection::GetInstance();
	pMakeMaterial->SetVertexShader(shCol->GetVertexShader("VS_UVScroll"));
	pMakeMaterial->SetPixelShader(shCol->GetPixelShader("PixelShader"));
	pShareMaterial = pMakeMaterial.get();	// マテリアルをもらう

	// 送る
	ResourceCollection::GetInstance()->SetResource<Material>("M_ObjectUI", std::move(pMakeMaterial));
}

void Object_UI::Update()
{
}

void Object_UI::SetupDraw()
{
	D3D11_Renderer* renderer = Direct3D11::GetInstance()->GetRenderer();
	// トポロジーを設定
	renderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// WVPのWとVを単位行列にする
	// 変換行列を作成
	// 移動行列
	Matrix t = XMMatrixTranslation(
		transform.position.x, 
		transform.position.y, 
		transform.position.z
	);

	// スケーリング行列
	Matrix s = XMMatrixScaling(
		transform.scale.x,
		transform.scale.y,
		transform.scale.z
	);

	// 回転行列
	Matrix r = Matrix::CreateFromYawPitchRoll(
		transform.rotation.y * Mathf::degToRad,
		transform.rotation.x * Mathf::degToRad,
		transform.rotation.z * Mathf::degToRad
	);

	Matrix worldMtx = s * r * t;	// ワールド変換行列を作成
	worldMtx.Transpose();

	RenderParam::WVP wvp;
	wvp.world = worldMtx;
	wvp.view = Matrix::Identity;
	wvp.view = wvp.view.Transpose();

	pShareMaterial->GetVertexShader().UpdateBuffer(0, &wvp);	// WVP行列を渡す
	pShareMaterial->GetPixelShader().SetTexture(0, pTexture);	// シェーダーにテクスチャセット

	// シェーダーを固定
	pShareMaterial->GetVertexShader().Bind();
	pShareMaterial->GetPixelShader().Bind();
}

void Object_UI::LateUpdate()
{
}

void Object_UI::Draw()
{
	SetupDraw();
	poligon->Draw();
}

#include "pch.h"
#include "Object_2D.h"
#include "ShaderCollection.h"

using namespace DirectX::SimpleMath;

void Object_2D::MaterialSetting()
{
	// 2D用マテリアルを取得する
	const std::string MATERIAL_NAME = "M_Object_2D";
	ResourceCollection* reCollect = ResourceCollection::GetInstance();
	pMaterial = reCollect->GetResource<MaterialClass>(MATERIAL_NAME);

	if (pMaterial == nullptr)	// マテリアルが無かったら作成
	{
		std::unique_ptr<MaterialClass> setMaterial = std::make_unique<MaterialClass>();
		ShaderCollection* shCol = ShaderCollection::GetInstance();

		// 使用するシェーダーをマテリアルに設定
		setMaterial->SetVertexShader(shCol->GetVertexShader(shCol->defaultVS));
		setMaterial->SetPixelShader(shCol->GetPixelShader(shCol->defaultPS));

		pMaterial = setMaterial.get();	// ポインタを受け取る
		// リソース管理に2D用マテリアルをセットする
		reCollect->SetResource<MaterialClass>(MATERIAL_NAME, std::move(setMaterial));
	}

}
void Object_2D::MakeVerticies()
{
	// テクスチャをはるポリゴンを生成する(TRIANGLESTRIP)
	// 頂点座標
	constexpr int vertexNum = 4;
	Vertex* pVerticies = new Vertex[vertexNum];
	pVerticies[0].position = { -0.5f, 0.5f, 0.0f };	// 左上
	pVerticies[1].position = { 0.5f, 0.5f, 0.0f };	// 右上
	pVerticies[2].position = { -0.5f, -0.5f, 0.0f };	// 左下
	pVerticies[3].position = { 0.5f, -0.5f, 0.0f };	// 右下

	// uv座標
	pVerticies[0].uv = { 0.0f, 0.0f };	// 左上
	pVerticies[1].uv = { 1.0f, 0.0f };	// 右上
	pVerticies[2].uv = { 0.0f, 1.0f };	// 左下
	pVerticies[3].uv = { 1.0f, 1.0f };	// 右下

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
	pSquareMesh = std::make_unique<Mesh>();
	pSquareMesh->SetVertexData(pVerticies, vertexNum, pIndicies, indexNum);
}

Object_2D::Object_2D() : pTexture(nullptr), pMaterial(nullptr)
{
	MaterialSetting();
	MakeVerticies();
}

Object_2D::~Object_2D()
{
}

void Object_2D::Draw()
{
	D3D11_Renderer* renederer = Direct3D11::GetInstance()->GetRenderer();
	DrawSetup(*renederer);	// 描画準備

	// メッシュ描画
	pSquareMesh->Draw(*renederer);
}

void Object_2D::DrawSetup(D3D11_Renderer& _renderer)
{
	// トポロジーを設定
	Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ワールド変換行列を求める
	Matrix worldMatrix = _renderer.GetWorldMtx(transform);
	RenderParam::WVP wvp = _renderer.GetParameter().GetWVP();
	wvp.world = worldMatrix;

	// マテリアルのパラメータを取得
	MaterialParameter& material = pMaterial->GetMaterialParameter();
	// 頂点シェーダー
	VertexShader& vtxSh = pMaterial->GetVertexShader();
	vtxSh.UpdateBuffer(0, &wvp);	// wvp行列を送る
	vtxSh.UpdateBuffer(1, &material);	// マテリアル情報を送る

	// ピクセルシェーダー
	if (material.isTextureEnable)
		pMaterial->GetPixelShader().SetTexture(0, pTexture);	//テクスチャを送る
	pMaterial->GetPixelShader().UpdateBuffer(1, &material);	// マテリアル情報を送る

	// シェーダーを固定
	pMaterial->GetVertexShader().Bind();
	pMaterial->GetPixelShader().Bind();
}

void Object_2D::SetTexture(Texture* _texture)
{
	pMaterial->GetMaterialParameter().isTextureEnable = true;
	pTexture = _texture;
}


//void Object_2D::SetSizeWidth(float _width)
//{
//	size.x = _width;
//	MakeVerticies();
//}
//
//void Object_2D::SetSizeHeight(float _height)
//{
//	size.y = _height;
//	MakeVerticies();
//}
//
//void Object_2D::SetSize(const DirectX::SimpleMath::Vector2& _size)
//{
//	size = _size;
//	MakeVerticies();
//}

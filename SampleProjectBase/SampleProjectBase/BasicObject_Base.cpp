#include "pch.h"
#include "BasicObject_Base.h"
#include "ShaderCollection.h"

using namespace DirectX::SimpleMath;

BasicObject_Base::BasicObject_Base() : color(Vector4::One)
{
	// 基本オブジェクト用のマテリアルを作成する
	ResourceCollection* reCol = ResourceCollection::GetInstance();
	// 既にあるか確認
	const std::string MATERIAL_NAME = "M_BaseObject";

	if (!reCol->GetImpotred(MATERIAL_NAME))	// 無かったら
	{
		// マテリアル作成
		std::unique_ptr<Material> makeMaterial = std::make_unique<Material>();
		// シェーダーを設定
		ShaderCollection* shCol = ShaderCollection::GetInstance();
		VertexShader* v = shCol->GetVertexShader("VertexShader");
		makeMaterial->pVertexShader = v;
		PixelShader* p = shCol->GetPixelShader("PS_VertexColor");
		makeMaterial->pPixelShader = p;

		material = makeMaterial.get();	// このオブジェクトにマテリアルセット
		// 管理クラスにセット
		reCol->SetResource(MATERIAL_NAME, std::move(makeMaterial));
	}
	else
	{
		// マテリアルを取得
		material = reCol->GetResource<Material>(MATERIAL_NAME);	
	}
}

BasicObject_Base::~BasicObject_Base()
{
}

void BasicObject_Base::Draw()
{
	D3D11_Renderer* renderer = Direct3D11::GetInstance()->GetRenderer();
	
	// 変換行列を作成
	// 移動行列
	DirectX::XMMATRIX moveMatrix = DirectX::XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z);
	// 拡大行列
	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);
	// 回転行列
	DirectX::XMMATRIX rotateMatX = DirectX::XMMatrixRotationX(transform.rotation.x);
	DirectX::XMMATRIX rotateMatY = DirectX::XMMatrixRotationY(transform.rotation.y);
	DirectX::XMMATRIX rotateMatZ = DirectX::XMMatrixRotationZ(transform.rotation.z);
	DirectX::XMMATRIX rotateMatrix = rotateMatX * rotateMatY * rotateMatZ;
	DirectX::XMMATRIX mtx = scaleMatrix
		* rotateMatrix
		* moveMatrix;
	
	// ワールド変換行列をWVPに入れる
	RenderParam::WVP wMat  = renderer->GetParameter().GetWVP();
	wMat.world = mtx;

	// シェーダーにバッファを送る
	material->pVertexShader->UpdateBuffer(0, &wMat);
	// シェーダーをセット
	material->pVertexShader->Bind();
	material->pPixelShader->Bind();

	Mesh::Draw(*renderer);

	transform.ResetParameter();	// 描画し終えたらリセット
	color = Vector4::One;	// 白色に戻す
}

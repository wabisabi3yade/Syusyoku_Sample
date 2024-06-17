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
	
	// ワールド変換行列を取得
	Matrix worldmtx = D3D11_Renderer::GetWorldMtx(transform);
	
	// ワールド変換行列をWVPに入れる
	RenderParam::WVP wMat  = renderer->GetParameter().GetWVP();
	wMat.world = worldmtx;

	// シェーダーにバッファを送る
	material->pVertexShader->UpdateBuffer(0, &wMat);
	// シェーダーをセット
	material->pVertexShader->Bind();
	material->pPixelShader->Bind();

	Mesh::Draw(*renderer);

	transform.ResetParameter();	// 描画し終えたらリセット
	color = Vector4::One;	// 白色に戻す
	color.w = 0.f;
}

void BasicObject_Base::SetColor(const DirectX::SimpleMath::Color& _color)
{
	color = _color;
}

#include "pch.h"
#include "BasicObject_Base.h"
#include "ShaderCollection.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"

using namespace DirectX::SimpleMath;

BasicObject_Base::BasicObject_Base() : pMaterial{ nullptr }
{
	// 基本オブジェクト用のマテリアルを作成する
	ResourceCollection* reCol = ResourceCollection::GetInstance();

	// 既にあるか確認
	const std::string MATERIAL_NAME = "M_BaseObject";

	// 無かったら
	if (!reCol->GetImpotred(MATERIAL_NAME))	
	{
		// マテリアル作成
		std::unique_ptr<Material> makeMaterial = std::make_unique<Material>();
		// シェーダーを設定
		ShaderCollection* shCol = ShaderCollection::GetInstance();
		VertexShader* v = shCol->GetVertexShader("VS_BaseObject");
		PixelShader* p = shCol->GetPixelShader("PS_VertexColor");
		makeMaterial->SetVertexShader(v);
		makeMaterial ->SetPixelShader(p);

		// このオブジェクトにマテリアルセット
		pBaseMaterial = makeMaterial.get();	

		// 管理クラスにセット
		reCol->SetResource(MATERIAL_NAME, std::move(makeMaterial));
	}
	else
	{
		// マテリアルを取得
		pBaseMaterial = reCol->GetResource<Material>(MATERIAL_NAME);	
	}
}

BasicObject_Base::~BasicObject_Base()
{
}

void BasicObject_Base::BasicDraw(Transform& _transform, DirectX::SimpleMath::Color& _color)
{
	D3D11_Renderer* renderer = Direct3D11::GetInstance()->GetRenderer();
	
	// ワールド変換行列を取得
	Matrix worldmtx = D3D11_Renderer::GetWorldMtx(_transform);
	
	// ワールド変換行列をWVPに入れる
	RenderParam::WVP wMat  = renderer->GetParameter().GetWVP();
	wMat.world = worldmtx;

	// シェーダーをセット
	pBaseMaterial->GetVertexShader().Bind();
	pBaseMaterial->GetPixelShader().Bind();

	// シェーダーにバッファを送る
	pBaseMaterial->GetVertexShader().UpdateBuffer(0, &wMat);
	pBaseMaterial->GetVertexShader().UpdateBuffer(1, &_color);

	Mesh::Draw();	
}

void BasicObject_Base::SetMaterial(Material& _material)
{
	pMaterial = &_material;
}

void BasicObject_Base::ImGuiSetting()
{
	if (pMaterial == nullptr) return;

	if (ImGui::TreeNode("Material"))
	{


		ImGui::TreePop();
	}
}

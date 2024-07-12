#pragma once
#include "CP_Renderer.h"

// 描画に必要
#include "Mesh_Base.h"

// メッシュ描画コンポーネント
class CP_MeshRenderer : public CP_Renderer
{
	Mesh_Base* pRenderMesh{ nullptr };

	// ワールド変換行列を求める
	const RenderParam::WVP WVPSetup();

	// マテリアルの描画準備
	/*void MeshDraw(u_int _meshIdx, RenderParam::WVP& _wvp);*/
public:
	using CP_Renderer::CP_Renderer;
	void Init();

	void Draw() override;

	// メッシュを設定
	void SetRenderMesh(Mesh_Base& _renderMesh);

	void ImGuiSetting() override;
};


#pragma once
#include "CP_Renderer.h"

// 描画に必要
#include "Mesh_Base.h"

// メッシュ描画コンポーネント
class CP_MeshRenderer : public CP_Renderer
{
	Mesh_Base* pRenderMesh{ nullptr };

public:
	using CP_Renderer::CP_Renderer;
	void Init();

	void Draw() override;

	// メッシュを設定
	void SetRenderMesh(Mesh_Base& _renderMesh);

	void ImGuiSetting() override;

private:

	/// @brief ワールド変換行列を求める
	/// @return 
	const RenderParam::WVP WVPSetup();

	/// @brief メッシュを描画
	void DrawMesh(RenderParam::WVP _wvp);
};


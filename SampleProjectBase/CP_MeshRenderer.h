#pragma once
#include "CP_Renderer.h"

// 描画に必要
#include "Mesh_Group.h"

// メッシュ描画コンポーネント
class CP_MeshRenderer : public CP_Renderer
{
	Mesh_Group* pRenderMesh{ nullptr };

public:
	using CP_Renderer::CP_Renderer;
	void Init();

	void Draw() override;

	void ImGuiSetting() override;

	/// @brief 描画するメッシュをセット
	/// @param _renderMesh メッシュ群
	void SetRenderMesh(Mesh_Group& _renderMesh);
	
	// シェーダーの名前からシェーダーを変更する
	void SetVertexShader(const std::string& _vsName);
	void SetPixelShader(const std::string& _psName);

	/// @brief 描画するメッシュを取得
	/// @return メッシュ群
	Mesh_Group* GetRenderMesh();

private:

	/// @brief メッシュを描画
	void DrawMesh(RenderParam::WVP _wvp);

	/// @brief マテリアルの準備
	/// @param _wvp wvp行列
	/// @param _mtrlIdx　マテリアルID
	void MaterialSetup(RenderParam::WVP& _wvp, u_int _mtrlIdx);

	/// @brief シェーダーの準備
	/// @param _shader シェーダー
	/// @param _wvp wvp行列
	/// @param _material マテリアル
	void ShaderSetup(Shader& _shader, RenderParam::WVP& _wvp, Material& _material);
};


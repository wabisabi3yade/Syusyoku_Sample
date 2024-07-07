#pragma once
#include "CP_Renderer.h"

// 描画に必要
#include "Mesh.h"
#include "Model.h"
#include "Material.h"

// 描画するメッシュの情報
class RenderMesh
{
	friend class CP_MeshRenderer;

	// メッシュ
	const Mesh* pMesh{ nullptr };

	// マテリアルの番号
	u_int materialID{ 0 };

public:
	RenderMesh() {}
	RenderMesh(const Mesh* _pMesh, u_int _materialID) : pMesh(_pMesh), materialID(_materialID) {};
	~RenderMesh() {}
};

// メッシュ描画コンポーネント
class CP_MeshRenderer : public CP_Renderer
{
	std::vector<RenderMesh> renderMeshes;

	std::vector<Material*> pMaterials;

	// ワールド変換行列を求める
	const RenderParam::WVP WVPSetup();

	// マテリアルの描画準備
	void MeshDraw(u_int _meshIdx, RenderParam::WVP& _wvp);
public:
	using CP_Renderer::CP_Renderer;
	void Init();

	void Draw() override;

	// モデルをセット
	void SetModel(Model& _model);

	// メッシュを設定
	void SetRenderMesh(const RenderMesh& _renderMesh);

	void ImGuiSetting() override;
};


#pragma once
#include "CP_Renderer.h"
#include "Mesh.h"
#include "Material.h"

// メッシュ描画コンポーネント
class CP_MeshRenderer : public CP_Renderer
{
	const Mesh* pMesh{nullptr};

	Material* pMaterial;

	void DrawSetup() override;
public:
	using CP_Renderer::CP_Renderer;
	void Init();

	void Draw() override;

	void SetMaterial(Material& _pMaterial) { pMaterial = &_pMaterial; }
	void SetMesh(const Mesh& _pMesh) { pMesh = &_pMesh; }

	void ImGuiSetting() override;
};


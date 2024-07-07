#pragma once
#include "CP_Renderer.h"
#include "Mesh.h"
#include "Material.h"

// ���b�V���`��R���|�[�l���g
class CP_MeshRenderer : public CP_Renderer
{
	std::vector<const Mesh*> pMeshes{ nullptr };

	std::vector<const Material*> pMaterials{ nullptr };

	// �`�揀��
	void DrawSetup() override;
public:
	using CP_Renderer::CP_Renderer;
	void Init();

	void Draw() override;

	void SetMaterial(std::vector<const Material*>& _pMaterial) { pMaterials = _pMaterial; }
	void SetMesh(std::vector<const Mesh*>& _pMesh) { pMeshes = _pMesh; }

	void ImGuiSetting() override;
};


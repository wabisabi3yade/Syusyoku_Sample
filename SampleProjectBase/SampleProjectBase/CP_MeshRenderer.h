#pragma once
#include "CP_Renderer.h"
#include "Mesh.h"
#include "MaterialClass.h"

// ���b�V���`��R���|�[�l���g
class CP_MeshRenderer : public CP_Renderer
{
	const Mesh* pMesh{nullptr};

	const Material* pMaterial;
public:
	using CP_Renderer::CP_Renderer;
	void Init();


	void Draw() override;

	void SetMaterial(const Material& _pMaterial) { pMaterial = &_pMaterial; }
};


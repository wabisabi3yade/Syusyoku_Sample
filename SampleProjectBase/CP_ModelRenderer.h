#pragma once
#include "CP_Renderer.h"

#include "Model.h"
#

// ���f���̕`��R���|�[�l���g
class CP_ModelRenderer : public CP_Renderer
{
	const Model* pModel{nullptr};

public:
	using CP_Renderer::CP_Renderer;

	void Draw()override;

	void SetModel(Model& _model) { pModel = &_model; }
};


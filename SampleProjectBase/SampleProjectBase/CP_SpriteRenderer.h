#pragma once
#include "CP_Renderer.h"

// �X�v���C�g��`�悷��R���|�[�l���g
class CP_SpriteRenderer : public CP_Renderer
{

public:
	using CP_Renderer::CP_Renderer;

	void Draw() override;
};


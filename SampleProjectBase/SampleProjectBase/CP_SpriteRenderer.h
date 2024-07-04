#pragma once
#include "CP_Renderer.h"

// スプライトを描画するコンポーネント
class CP_SpriteRenderer : public CP_Renderer
{

public:
	using CP_Renderer::CP_Renderer;

	void Draw() override;
};


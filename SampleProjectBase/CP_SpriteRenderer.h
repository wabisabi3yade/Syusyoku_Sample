#pragma once
#include "CP_Renderer.h"

// 描画関係
#include "Sprite.h"
#include "Material.h"

// スプライトを描画するコンポーネント
class CP_SpriteRenderer : public CP_Renderer
{
	// スプライト
	std::unique_ptr<Sprite> pSprite;

	// マテリアル
	Material* pMaterial;

	void Copy(const CP_SpriteRenderer& _other);

	// 描画準備
	void DrawSetup();

	// マテリアルの準備
	void MaterialSetup();
public:
	using CP_Renderer::CP_Renderer;

	CP_SpriteRenderer& operator=(const CP_SpriteRenderer& _other);

	void Init() override;

	void Draw() override;

	void SetTexture(Texture& _texture);

	void SetMaterial(Material& _material);
};


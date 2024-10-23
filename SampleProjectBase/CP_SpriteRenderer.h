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
	Material* pMaterial{ nullptr };

	// マテリアルの準備
	void MaterialSetup();

	// 描画準備
	void DrawSetup();

public:
	CP_SpriteRenderer();
	CP_SpriteRenderer(const CP_SpriteRenderer& _other);
	CP_SpriteRenderer& operator=(const CP_SpriteRenderer& _other);
	
	void Init() override;

	void Draw() override;

	void ImGuiSetting() override;

	void SetTexture(Texture& _texture);

	void SetMaterial(Material& _material);

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:

	void Copy(const CP_SpriteRenderer& _other);
};


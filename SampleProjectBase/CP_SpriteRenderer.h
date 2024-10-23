#pragma once
#include "CP_Renderer.h"

// �`��֌W
#include "Sprite.h"
#include "Material.h"

// �X�v���C�g��`�悷��R���|�[�l���g
class CP_SpriteRenderer : public CP_Renderer
{
	// �X�v���C�g
	std::unique_ptr<Sprite> pSprite;

	// �}�e���A��
	Material* pMaterial{ nullptr };

	// �}�e���A���̏���
	void MaterialSetup();

	// �`�揀��
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


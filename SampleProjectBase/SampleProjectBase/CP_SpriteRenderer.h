#pragma once
#include "CP_Renderer.h"
#include "Sprite.h"

class Material;

// �X�v���C�g��`�悷��R���|�[�l���g
class CP_SpriteRenderer : public CP_Renderer
{
	// �X�v���C�g
	std::unique_ptr<Sprite> pSprite;

	// �}�e���A��
	Material* pMaterial;

	void Copy(const CP_SpriteRenderer& _other);

	// �`�揀��
	void DrawSetup()override;

	// �}�e���A���̏���
	void MaterialSetup();
public:
	using CP_Renderer::CP_Renderer;

	CP_SpriteRenderer& operator=(const CP_SpriteRenderer& _other);

	void Init() override;

	void Draw() override;

	void SetTexture(Texture& _texture);

	void SetMaterial(Material& _material);
};


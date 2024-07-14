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

	/// @brief �e�N�X�`�����ݒ肳��Ă��邩
	bool isTextureEnable{ false };

	// �}�e���A���̏���
	void MaterialSetup();

	// �`�揀��
	void DrawSetup();

public:
	using CP_Renderer::CP_Renderer;

	CP_SpriteRenderer& operator=(const CP_SpriteRenderer& _other);

	void Init() override;

	void Draw() override;

	void SetTexture(Texture& _texture);

	void SetMaterial(Material& _material);

private:

	void Copy(const CP_SpriteRenderer& _other);
};


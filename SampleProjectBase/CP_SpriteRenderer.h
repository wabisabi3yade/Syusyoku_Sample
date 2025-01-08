#pragma once
#include "CP_Renderer.h"

// �`��֌W
#include "Sprite.h"
#include "Material.h"

namespace HashiTaku
{
	// �X�v���C�g��`�悷��R���|�[�l���g
	class CP_SpriteRenderer : public CP_Renderer
	{
		// �X�v���C�g
		std::unique_ptr<Sprite> pSprite;

		/// @brief ���l
		float alpha;

		// �}�e���A��
		Material* pMaterial{ nullptr };

		/// @brief �`�悷�钸�_�V�F�[�_�[
		VertexShader* pDrawVS;

		/// @brief �`�悷��s�N�Z���V�F�[�_�[
		PixelShader* pDrawPS;

		// �}�e���A���̏���
		void MaterialSetup();

		// �`�揀��
		void DrawSetup();

	public:
		CP_SpriteRenderer();
		~CP_SpriteRenderer(){}

		void Init() override;

		void Draw() override;

		void SetTexture(Texture& _texture);

		void SetMaterial(Material& _material);

		/// @brief ���l���Z�b�g
		/// @param _alpha ���l
		void SetAlpha(float _alpha);

		/// @brief ���l���擾����
		/// @return ���l
		float GetAlpha() const;

		json Save() override;
		void Load(const json& _data) override;

	private:
		void ImGuiDebug() override;

		// �g�p����V�F�[�_�[��ImGui��
		void ImGuiUseShader();
	};
}
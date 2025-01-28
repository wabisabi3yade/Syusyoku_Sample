#pragma once
#include "CP_Renderer.h"
#include "Polygon2D.h"

namespace HashiTaku
{
	class VertexShader;
	class PixelShader;
	class Texture;

	class CP_UIRenderer : public CP_Renderer
	{
		/// @brief UI�`��Ɏg�p���钸�_�V�F�[�_�[
		static VertexShader* pVertexShader;

		/// @brief UI�`��Ɏg�p����s�N�Z���V�F�[�_�[
		static PixelShader* pPixelShader;

		/// @brief �V�F�[�_�[�����ɃZ�b�g����Ă��邩�H
		static bool isSetShader;

		/// @brief �\������e�N�X�`��
		const Texture* pTexture;

		/// @brief ���l
		float alpha;

	protected:
		/// @brief �e�N�X�`����\��|���S��
		std::unique_ptr<Polygon2D> pPolygon;

	public:
		CP_UIRenderer();
		virtual ~CP_UIRenderer() {}

		void Init() override;
		void OnChangeScale();
		void OnChangePosition();
		void OnChangeRotation();

		/// @brief �e�N�X�`�����Z�b�g
		/// @param _pSetTexture �Z�b�g����e�N�X�`��
		void SetTexture(const Texture* _pSetTexture);

		/// @brief UV���W���Z�b�g
		/// @param _startUV �J�nUV
		/// @param _endUV �I�_UV
		void SetUV(const DXSimp::Vector2& _startUV, const DXSimp::Vector2& _endUV);

		/// @brief ���l���Z�b�g
		/// @param _alpha ���l
		void SetAlpha(float _alpha);

		/// @brief ���l���Z�b�g
		/// @return ���l
		float GetAlpha() const;

		/// @brief �Z�[�u����
		/// @param _data �Z�[�u�V�[���f�[�^
		json Save() override;

		/// @brief ���[�h����
		/// @param _data ���[�h����V�[���f�[�^ 
		void Load(const json& _data) override;

	protected:
		/// @brief �T�C�Y�ȂǕύX���邽�߂Ƀ|���S���Đ���(�g�����X�t�H�[�����玩����)
		virtual void ReCreatePolygon();

		void ImGuiDebug() override;
	private:
		/// @brief �`��
		void Draw() override;

		/// @brief �`�揀��
		void DrawSetup();
	};
}
#pragma once
#include "Texture.h"
#include "PlaneMesh.h"

#include "ISaveLoad.h"

namespace HashiTaku
{
	class Material;

	// �l�p�`�|���S���Ƀe�N�X�`����\���ĕ\������N���X
	class Sprite : public ISaveLoad
	{
		/// @brief �e�N�X�`��
		Texture* pTexture;

		/// @brief �l�p�`�|���S��
		std::unique_ptr<PlaneMesh> pSquareMesh;

		/// @brief uv���W�̎n�_(����)
		DXSimp::Vector2 uvStart;

		/// @brief uv���W�̏I�_(�E��)
		DXSimp::Vector2 uvEnd;

		/// @brief uv�̕�����
		DXSimp::Vector2 uvDivideNum;

		/// @brief �e�N�X�`�����ݒ肳��Ă��邩
		bool isTextureEnable{ false };

		/// @brief �R�s�[�֐�
		/// @param _other �R�s�[�����Ώ�
		void Copy(const Sprite& _other);
	public:
		Sprite();
		Sprite(const Sprite& _other);
		~Sprite() {};

		Sprite& operator=(const Sprite& _other);

		/// @brief �e�N�X�`�����Z�b�g
		/// @param _tex �e�N�X�`���̎Q��
		void SetTexture(Texture& _tex);

		/// @brief �e�N�X�`�����擾
		/// @return �e�N�X�`���̎Q��
		Texture* GetTexture() const;

		/// @brief �l�p�`�|���S�����擾
		/// @return �l�p�`�|���S����const�Q��
		const PlaneMesh& GetSquare() const;

		// �e�N�X�`���g�p���Ă��邩�Ԃ�
		bool GetIsTexEnable() const;

		/// @brief uv���W��ݒ�
		/// @param _uvStart uv���W�n�_
		/// @param _uvEnd uv���W�I�_
		void SetUV(const DXSimp::Vector2& _uvStart, const DXSimp::Vector2& _uvEnd);

		/// @brief �Z�[�u����
		/// @param _sceneData �Z�[�u�f�[�^
		json Save() override;

		/// @brief ���[�h����
		/// @param _sceneData ���[�h����f�[�^ 
		void Load(const json& _data) override;
	};
}
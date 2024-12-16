#pragma once
#include "Component.h"
#include "PlaneMesh.h"
#include "Sprite.h"
#include "SpriteDrawer.h"

namespace HashiTaku
{
	class Texture;
	class Material;
	class CP_Camera;

	/// @brief �X�J�C�{�b�N�X��`�悷��
	class CP_SkyBox : public Component
	{
	public:
		/// @brief �{�b�N�X�̖ʂ̏ꏊ
		enum class FaceKind
		{
			X_Right,
			X_Left,
			Y_Top,
			Y_Bottom,
			Z_Front,
			Z_Back,
			MaxNum
		};

	private:
		/// @brief �X�J�C�{�b�N�X�̖�
		std::unique_ptr<Sprite> pFaces[static_cast<int>(FaceKind::MaxNum)];

		/// @brief �X�v���C�g�`��
		std::unique_ptr<SpriteDrawer> pSpriteDrawer;

		/// @brief �g�p����}�e���A��
		Material* pMaterial;

		/// @brief �X�J�C�{�b�N�X�̃T�C�Y
		float size;
	public:
		CP_SkyBox();
		~CP_SkyBox() {}
		CP_SkyBox(const CP_SkyBox& _other);

		CP_SkyBox& operator=(const CP_SkyBox& _other);

		void Init() override;
		void Draw() override;
		void ImGuiDebug() override;

		/// @brief �w�肵���ʂ̃e�N�X�`����\��
		/// @param _pTex �e�N�X�`��
		/// @param _setFace �w�肵����
		void SetTexture(Texture& _pTex, FaceKind _setFace);

		/// @brief �Z�[�u����
		/// @param _data �Z�[�u�V�[���f�[�^
		nlohmann::json Save() override;

		/// @brief ���[�h����
		/// @param _data ���[�h����V�[���f�[�^ 
		void Load(const nlohmann::json& _data) override;
	private:
		/// @brief �}�e���A���̃Z�b�g�A�b�v
		void MaterialSetting();

		/// @brief �`��ł��邩�`�F�b�N
		/// @return �`��ł��邩�H
		bool IsCanDraw();

		/// @brief 1�ʂ�`�悷��
		/// @param _faceNum �`�悷���
		void FaceDraw(u_int _faceNum);

		/// @brief �ʂ̏����擾����
		/// @param _facePlace �ʂ̈ʒu
		/// @param _outPos ���W
		/// @param _outScale �X�P�[��
		/// @param _outAngles �p�x
		void FaceTransform(FaceKind _facePlace, DirectX::SimpleMath::Vector3& _outPos, DirectX::SimpleMath::Vector3& _outScale, DirectX::SimpleMath::Vector3& _outAngles);

		void Copy(const CP_SkyBox& _other);

		/// @brief �e�N�X�`�����Z�[�u����
		/// @param _data �ۑ�����f�[�^��
		/// @param _kind �ʂ̎��
		void SaveTexture(nlohmann::json& _data, FaceKind _kind);

		void LoadTexture(const nlohmann::json& _data, FaceKind _kind);
	};
}
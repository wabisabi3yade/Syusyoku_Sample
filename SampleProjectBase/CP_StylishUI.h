#pragma once
#include "Component.h"

namespace HashiTaku
{
	class CP_UIRenderer;
	class CP_UISlider;

	/// @brief �X�^�C���b�V�������N�𐧌䂷��N���X
	class CP_StylishUI : public Component
	{
		/// @brief �����N
		enum class StylishRank
		{
			None,	// �Ȃɂ��Ȃ�(��ԉ�)
			D,
			C,
			B,
			A,
			S,
			SS,
			SSS,
			MaxNum
		};

		/// @brief �����N���Ƃ̃p�����[�^
		struct RankParameter
		{
			/// @brief ��ʂɕ\������e�N�X�`��
			const Texture* displayTexture{ nullptr };

			/// @brief ���̃����N�ɓ��邽�߂̃|�C���g��
			float requireEnterPoints{ 0.0f };

			/// @brief ����Ƃ��̃|�C���g���x
			float decadePointSpeed{ 100.0f };
		};

		/// @brief �����N�̃p�����[�^���X�g
		std::array<RankParameter, static_cast<u_int>(StylishRank::MaxNum)> rankParameters;

		/// @brief �����N�`��I�u�W�F�N�g��
		std::string rankUIRendererName;

		/// @brief �X�^�C���b�V���Q�[�W�I�u�W�F�N�g��
		std::string stylishGageName;

		/// @brief UI�`��
		CP_UIRenderer* pUIRenderer;

		/// @brief �X�^�C���b�V���Q�[�W
		CP_UISlider* pStylishGage;

		/// @brief ���݂̃X�^�C���b�V�������N
		StylishRank curStylishRank;

		/// @brief ���݂̃X�^�C���b�V���|�C���g
		float curStylishPoint;

		/// @brief ���̎��Ԃ��o�߂����猸���Ă����悤�ɂ���
		float decadeBeginTime;

		/// @brief ����܂ł̌o�ߎ���
		float elapsedToDecadeTime;

		/// @brief �|�C���g�����葱���Ă��邩�H
		bool isPointDecading;
	public:
		CP_StylishUI();
		~CP_StylishUI() {}

		/// @brief �X�^�C���b�V���|�C���g�����Z
		/// @param _addPoint ���Z����|�C���g 
		void AddStylishPoint(float _addPoint);

		/// @brief �X�^�C���b�V���|�C���g�����Z
		/// @param _decadePoint ���Z����|�C���g 
		void DecadeStylishPoint(float _decadePoint);
	private:
		void Update() override;

		/// @brief �|�C���g�����炷�X�V����
		void DecadePointUpdate();

		/// @brief ���邽�߂̃����N�|�C���g���擾
		/// @param _checkRank �m�F�����������N
		/// @return �����N�|�C���g
		float GetEnterRankPoint(StylishRank _checkRank) const;

		/// @brief �����N��ύX����
		void ChangeRank(StylishRank _targetRank);

		/// @brief ���݂̃|�C���g���Q�[�W�ɔ��f
		void ApplyStylishGage();

		/// @brief �����ł��邩�擾
		/// @return �����ł��邩�H
		bool GetCanActive() const;

		/// @brief ���̃����N���擾����
		/// @param _targetRank ���̃����N�������������N
		/// @return ���̃����N
		StylishRank GetNextRank(StylishRank _targetRank) const;

		/// @brief �O�̃����N���擾����
		/// @param _targetRank �O�̃����N�������������N
		/// @return �O�̃����N
		StylishRank GetPrevRank(StylishRank _targetRank) const;

		/// @brief ���ݍő�̃����N��Ԃ��擾����
		/// @return ���ݍő�̃����N��Ԃ��H
		bool GetIsCurMaxRank() const;

		void ImGuiDebug() override;
	};
}


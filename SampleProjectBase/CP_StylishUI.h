#pragma once
#include "Component.h"
#include "PerlinShake.h"

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
			None = 0,	// �Ȃɂ��Ȃ�(��ԉ�)
			D,
			C,
			B,
			A,
			S,
			SS,
			SSS,
			MaxNum
		};

		enum class RankupDirectionState
		{
			Wait,	// �ҋ@
			Scaling,	// �X�P�[�����O
			Shaking	// �V�F�C�N
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

		/// @brief �����N�オ�����Ƃ��ɕ������V�F�C�N����Ƃ��̌v�Z�N���X
		std::unique_ptr<PerlinShake> pRankupShakeVector;

		/// @brief �����N�オ�����Ƃ��̃V�F�C�N�p�����[�^
		PerlinShakeParameter rankupShakeParam;

		/// @brief �X�^�C���b�V���Q�[�W�I�u�W�F�N�g��
		std::string stylishGageName;

		/// @brief �X�^�C���b�V���Q�[�W�O���[�v�I�u�W�F�N�g��
		std::string gageGroupObjName;

		/// @brief �����N�`��
		CP_UIRenderer* pStylishRankRenderer;

		/// @brief �X�^�C���b�V���Q�[�W
		CP_UISlider* pStylishGage;

		/// @brief �Q�[�W�̃O���[�v�I�u�W�F�N�g
		GameObject* pGageGroupObj;

		/// @brief ���݂̃X�^�C���b�V�������N
		StylishRank curStylishRank;

		/// @brief �����NUI�̊�b���W
		DXSimp::Vector3 rankUIBasePos;

		/// @brief �����NUI�̊�b�X�P�[��
		DXSimp::Vector3 rankUIBaseScale;

		/// @brief ���݂̃X�^�C���b�V���|�C���g
		float curStylishPoint;

		/// @brief ���̎��Ԃ��o�߂����猸���Ă����悤�ɂ���
		float decadeBeginTime;

		/// @brief ����܂ł̌o�ߎ���
		float elapsedToDecadeTime;

		/// @brief ���݂̃����N���o���
		RankupDirectionState curRankupDirState;

		/// @brief �����N�オ�����Ƃ��Ɏg�p����C�[�W���O
		EaseKind rankupEase;

		/// @brief �����N�オ�����Ƃ��̃X�P�[���ύX����
		float rankupScalingTime;
		
		/// @brief �����N�̏オ�����Ƃ��̉��o�ł̊J�n�X�P�[���{��
		float rankupScalingRatio;

		/// @brief �����N�オ�����Ƃ��̉��o�o�ߎ���
		float elapsedRankupDirectionTime;
	public:
		CP_StylishUI();
		~CP_StylishUI() {}

		/// @brief �X�^�C���b�V���|�C���g�����Z
		/// @param _addPoint ���Z����|�C���g 
		void AddStylishPoint(float _addPoint);

		/// @brief �X�^�C���b�V���|�C���g�����Z
		/// @param _decadePoint ���Z����|�C���g 
		void DecadeStylishPoint(float _decadePoint);

		/// @brief �Z�[�u����
		/// @param _data �Z�[�u�V�[���f�[�^
		json Save() override;

		// �����N�p�����[�^�̃Z�[�u����
		json SaveRankParameters();

		/// @brief ���[�h����
		/// @param _data ���[�h����V�[���f�[�^ 
		void Load(const json& _data) override;

		// �����N�p�����[�^�̃��[�h����
		void LoadRankParameters(const json& _rankParamData);
	private:
		void Start() override;
		void Update() override;

		/// @brief �����N�オ�����Ƃ��ɕK�v�ȃp�����[�^�̏�����
		void InitRankupParam();

		/// @brief �K�v�ȃI�u�W�F�N�g���擾
		void FindRequireObject();

		/// @brief �|�C���g�����炷�X�V����
		void DecadePointUpdate();

		/// @brief �����N�オ�����Ƃ��̍X�V����
		void RankupDirectionUpdate(); 

		/// @brief �����N�オ�����Ƃ��̃X�P�[�����O����
		void RankupScalingUpdate();
	
		/// @brief �����N�オ�����Ƃ��̃V�F�C�N����
		void RankupShakeUpdate();

		/// @brief ���邽�߂̃����N�|�C���g���擾
		/// @param _checkRank �m�F�����������N
		/// @return �����N�|�C���g
		float GetEnterRankPoint(StylishRank _checkRank) const;

		/// @brief �����N��ύX����
		void ChangeRank(StylishRank _targetRank);

		/// @brief ���݂̃|�C���g���Q�[�W�ɔ��f
		void ApplyStylishGage();

		/// @brief �����N���オ�����Ƃ��̉摜�̉��o
		void BeginRankUpDirection();

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

		/// @brief ���݂̃����N���|�C���g����擾����
		/// @return ���݂̃����N
		StylishRank GetCurrentRankFromPoint();

		/// @brief ���ݍő�̃����N��Ԃ��擾����
		/// @return ���ݍő�̃����N��Ԃ��H
		bool GetIsCurMaxRank() const;

		void ImGuiDebug() override;

		// �����N�オ�����Ƃ��֘A
		void ImGuiRankupDirection();

		// �����N�p�����[�^�ҏW
		void ImGuiRankParameter();

		// �I�u�W�F�N�g���ҏW
		void ImGuiSetObject();
	};
}


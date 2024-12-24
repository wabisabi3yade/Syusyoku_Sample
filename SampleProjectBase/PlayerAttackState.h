#pragma once
#include "PlayerGroundState.h"
#include "PlayerAttackInformation.h"
#include "AnimationCurve.h"

namespace HashiTaku
{
	/// @brief �v���C���[�U���̃Q�[��������
	class PlayerAttackState : public PlayerGroundState
	{
	private:
		/// @brief �R���r�l�[�V�����U���̐�
		PlayerState nextCombAtkState;

		/// @brief �U����񃊃X�g(�P���Ȃ�ŏ��̏����g�p����)
		std::vector<PlayerAttackInformation> attackInfos;

		/// @brief �O�ɐi�ދ����J�[�u
		AnimationCurve progressDistanceCurve;

		/// @brief ���A�^�b�N�ϐ�
		const bool* pIsReAttack;

		/// @brief ����̍U���Ői�ދ���
		float curAtkProgressDis;

		/// @brief �i�ލő勗��
		float atkMaxDistance;

		/// @brief �O�t���[���܂łɐi�񂾋���
		float prevProgressDistance;

		/// @brief �U����񉽌ڂ�
		u_int curAttackTime;

		/// @brief �S�̂̍U����񉽌��邩
		u_int attackTimeCnt;

		/// @brief �O�֐i�ނ��H
		bool isMoveForward;
	public:
		PlayerAttackState();
		virtual ~PlayerAttackState() {}

		json Save() override;
		void Load(const json& _data) override;
	protected:
		// State���ʏ���
		void OnStartBehavior() override;
		void UpdateBehavior() override;
		void OnEndBehavior() override;

		void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName) override;

		/// @brief �U���񐔂��Z�b�g
		void SetAttackTimes(u_int _attackTimes);

		/// @brief �U���񐔂�ς����Ƃ��̏���
		virtual void OnChangeAttackTimes();

		/// @brief �w�肵���U�������擾
		/// @param _atkIdx �z��̗v�f��
		/// @return �U�����(�͈͊O�Ȃ�nullptr)
		PlayerAttackInformation* GetPlayerAtkInfo(u_int _atkIdx);

		/// @brief �U���񐔂��擾
		/// @return �U����
		u_int GetAttackTimes() const;

		void ImGuiDebug() override;

		// �U�����̕ҏW
		virtual void ImGuiAttackInfo();
	private:
		/// @brief �p�����[�^��������
		void InitParameter();

		/// @brief �U�������X�V����
		void UpdateAttackInfo();

		/// @brief ���A�^�b�N���邩�m�F����
		void UpdateReAttack();

		/// @brief �R���r�l�[�V�����U���̓��͍X�V
		void UpdateCombInput();

		/// @brief �U���Ői�ދ��������߂�
		/// @param _atkEnemyPos �G�̍��W
		void CalcProgressDis(const DirectX::SimpleMath::Vector3& _atkEnemyPos);

		/// @brief �G�ɑ΂��Č�����
		/// @param _atkEnemyPos �G�̍��W
		void LookAtEnemyInstant(DirectX::SimpleMath::Vector3 _atkEnemyPos);

		/// @brief �U�����ɑO�֐i��
		void ForwardProgressMove();

		void ImGuiCombAttack();
	private:
#ifdef EDIT
		// �R���r�l�[�V�����U�����ł���X�e�[�g
		inline static std::vector<std::string> combAtkState
		{
			"Attack11",
			"Attack12",
			"Attack13",
			"Attack14"
		};
#endif // EDIT
	};
}
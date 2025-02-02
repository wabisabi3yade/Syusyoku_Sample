#pragma once
#include "BossActState_Base.h"
#include "BossAttackInformation.h"

namespace HashiTaku
{
	/// @brief �{�X��s�U���X�e�[�g
	class BossAttackState : public BossActState_Base
	{
		/// @brief ��]���x�J�[�u
		AnimationCurve rotSpeedCurve;

		/// @brief ��]���x�{��
		float rotSpeedTimes;

		/// @brief �R���r�l�[�V�����U���̍U����
		u_int attackTimeCnt;

		/// @brief ���݁A�U���̉��i�ڂ�
		u_int curAttackTime;

		/// @brief �J�[�u�ŉ�]�ړ������邩�H
		bool isUseRotateCurve;

	protected:
		/// @brief �U����񃊃X�g(�P���Ȃ�ŏ��̏����g�p����)
		std::vector<std::unique_ptr<BossAttackInformation>> attackInfos;

	public:
		BossAttackState();
		virtual ~BossAttackState() {}

		/// @brief �X�e�[�g�̏�����
		void InitState() override;

		/// @brief �J�n
		void OnStartBehavior() override;

		/// @brief �X�V����
		void UpdateBehavior() override;

		/// @brief �A�j���[�V�����I�����̍s��
		/// @param _fromAnimNodeName �J�ڌ��̃A�j���[�V�����m�[�h��
		/// @param _toAnimNodeName �J�ڐ�̃A�j���[�V�����m�[�h��
		void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName) override;

		json Save() override;
		void Load(const json& _data) override;
	protected:
		/// @brief �������X�V����
		void RotateUpdate();

		/// @brief �U�������쐬
		/// @return �U�����
		std::unique_ptr<BossAttackInformation> CreateAttackInfo();

		void ImGuiDebug() override;
	private:
		/// @brief �R���r�l�[�V�����U���̍U���ɍ��킹�čU�������X�V����
		void UpdateReAttack();

		/// @brief �U���񐔂��Z�b�g
		/// @param _attackTime �U����
		void SetAttackTimeCnt(u_int _attackTime);
	};
}
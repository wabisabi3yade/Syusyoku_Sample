#pragma once
#include "PlayerActState_Base.h"

namespace HashiTaku
{
	class PlayerAirActionController;

	/// @brief �v���C���[�󒆍s���̃x�[�X
	class PlayerAirState : public PlayerActState_Base
	{
	public:
		/// @brief  �v���C���[�̍s�����
		enum class PlayerState
		{
			// �ҋ@
			Idle = 1,

			// �ړ�
			Move,
			Damage,	// �_���[�W�̂�����
			Guard,	// �K�[�h�J�n

			// �R���r�l�[�V�����U��
			Attack11 = 20,
			Attack12,
			Attack13,

			// �K�E�U��
			HelmBreaker = 40,	// ������
			SpecialAtkGuard,	// �K�[�h�p���B���̍U��

			// ����
			None = 99
		};

	public:
		/// @brief �X�e�[�g���̃_���[�W����
		/// @param _attackInfo �󂯂��U�����
		/// @return �_���[�W�������s�����H
		bool OnDamage(AttackInformation& _attackInfo) override;

	protected:
		/// @brief �J�n�����Ăяo��
		void OnStart() override;

		/// @brief �X�V�����Ăяo��
		void Update() override;

		/// @brief  �I�������Ăяo��
		void OnEnd() override;

		/// @brief �e��Ԃ̊J�n����
		virtual void OnStartBehavior() {};

		/// @brief �e��Ԃ̍X�V����
		virtual void UpdateBehavior() {};

		/// @brief �e��Ԃ̏I������
		virtual void OnEndBehavior() {};

		/// @brief �X�e�[�g�J�ڏ����̃`�F�b�N����
		virtual void TransitionCheckUpdate();

		/// @brief �󒆍s���̃R���g���[���[�ɃL���X�g
		/// @return �󒆍s���R���g���[���[
		PlayerAirActionController& CastAirController();

		/// @brief ��Ԃ�J�ڂ���
		/// @param _changeSate �J�ڐ�̏��
		/// @param _isForce) �����I��
		void ChangeState(PlayerState _nextState, bool _isForce = false);

		void ImGuiDebug() override;

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief ���͍X�V
		void InputUpdate();

	protected:
		// �󒆍s������߂�Ƃ���A�j���[�V������
		static constexpr auto FALLLOOP_ANIM_NAME{ "Fall_Loop" };
	};
}
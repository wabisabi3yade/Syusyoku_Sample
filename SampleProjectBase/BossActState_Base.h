#pragma once
#include "EnemyActState_Base.h"
#include "CP_Animation.h"
#include "CP_RigidBody.h"

namespace HashiTaku
{
	class BossActionController;

	/// @brief �{�X�̃X�e�[�g�m�[�h���
	class BossActState_Base : public EnemyActState_Base
	{
	protected:
		/// @brief  ���[�v���[�V�����ŕK�v�ȃp�����[�^
		struct WarpMotionParam
		{
			/// @brief ���[�v���[�V�����ňړ����銄��(XZ����)
			std::unique_ptr<AnimationCurve> pHoriMovementCurve;

			/// @brief �ړ�����XZ�ő勗��(0�����Ȃ琧���Ȃ��Ƃ���)
			float maxMovementXZ{ -10.0f };

			/// @brief �^�[�Q�b�g�Ƃ�����W����̍����֌������悤�ɂ���I�t�Z�b�g
			float targetPosOffset{ 0.0f };

			/// @brief �J�n���銄��
			float beginAnimRatio{ 0.0f };

			/// @brief �I�����銄��
			float endAnimRatio{ 1.0f };

			/// @brief ���[�g���[�V�����̊�������ړ����������߂�
			bool isFromRootMotion{ true };

			/*/// @brief ���[�v���[�V�����ňړ����銄��(Y����)
			AnimationCurve vertMovementCurve;*/

			/*	/// @brief �ړ�����Y�ő勗��(0�����Ȃ琧���Ȃ��Ƃ���)
			float maxMovementY{ -10.0f };*/

			/*/// @brief Y�ړ������邩�H
			bool isUseVertical{ false };*/
		};

	public:
		// �{�X�̃X�e�[�g�^�C�v(��̒ǉ���)
		enum class BossState
		{
			// �ړ�
			Idle,
			Walk,
			Run,

			// �U�镑��
			Damage_Small,	// ���_���[�W
			BreakEnd_Knock,	// �u���C�N�I���m�b�N
			Break_Idle,	// �u���C�N���ҋ@

			// �U��
			CombAttack1 = 100,	// �R���r�l�[�V�����U��
			JumpAttack,	// �W�����v�U��
			SlidingAttack,	// �X���C�f�B���O�U��
			CombAttack2,	// �R���r�l�[�V�����U��
			CombAttack3,	// �R���r�l�[�V�����U��

			// �Ō�
			None
		};

	private:
		/// @brief ���[�v���[�V�����̃p�����[�^�S
		std::vector<WarpMotionParam> warpMotionParams;

		/// @brief ���[�v���[�V�����̃^�[�Q�b�g��̃|�C���^
		const DXSimp::Vector3* pWarpTargetPos;

		/// @brief ���[�v���[�V�����̃^�[�Q�b�g����W
		DXSimp::Vector3 warpTargetPos;

		/// @brief ���[�v���[�V�����ňړ�����Ƃ��̃^�[�Q�b�g���W�Ƃ̋���
		DXSimp::Vector3 disToWarpTargePos;

		/// @brief �Ō�̃��[�v���[�V�����ŋ��߂��i�s�x(0.0�`1.0)
		float lastProgressRatio;

		/// @brief �{�X�̃X�e�[�g
		BossState stateType;

		/// @brief �S�Ăŉ��񃏁[�v�����邩�H
		u_int allWarpSteps;

		/// @brief ���݂̃��[�v��
		u_int curWarpStep;

		/// @brief ���[�v�J�n���̃��[�g���[�V�������W
		DXSimp::Vector3 startRMPos;

		/// @brief ���[�v�I�����̃��[�g���[�V�������W
		DXSimp::Vector3 endRMPos;

		/// @brief ���[�v���[�V���������邩�H
		bool isUseWarpMotion;

		/// @brief ���[�v���[�V���������H
		bool isWarpMoving;

#ifdef EDIT
		// ���[�v���[�V�����J�n���̍��W
		DXSimp::Vector3 warpStartPos;
#endif
	public:
		BossActState_Base();
		virtual ~BossActState_Base() {}

		/// @brief ����������
		/// @param _stateType�@���
		/// @param _actController�@�A�N�V�����R���g���[���[
		void Init(BossState _stateType, BossActionController& _actController);

		/// @brief ��Ԑ؂�ւ��J�n����
		void OnStart() override;

		/// @brief �X�V����
		void Update() override;

		/// @brief ��Ԑ؂�ւ��I������
		void OnEnd() override;

		/// @brief �{�X�̃A�N�V�����R���g���[���[���擾
		/// @return �{�X�̃A�N�V�����R���g���[���[
		BossActionController& GetBossActionController();

		/// @brief �_���[�W������
		virtual void OnDamage();

		/// @brief �f�o�b�O�`��
		void DebugDisplay();

		json Save() override;
		void Load(const json& _data) override;
	protected:
		/// @brief �X�e�[�g�̏�����
		virtual void InitState() {}

		/// @brief ��Ԑ؂�ւ��J�n����
		virtual void OnStartBehavior() {}

		/// @brief �X�V����
		virtual void UpdateBehavior() {}

		/// @brief ��Ԑ؂�ւ��I������
		virtual void OnEndBehavior() {}

		/// @brief �J�ڐ؂�ւ��m�F
		virtual void TransitionCheckUpdate();

		/// @brief �J�ڂ���
		/// @param _nextState ���̑J��
		/// @param _isForce�@�����؂�ւ�
		void ChangeState(BossState _nextState, bool _isForce = false);

		/// @brief ���[�v���[�V�����̃^�[�Q�b�g�̍��W�̎Q�Ƃ��Z�b�g
		/// @param _targetWorldPos �^�[�Q�b�g�̍��W�̎Q��
		void SetWarpTargetPosReference(const DXSimp::Vector3& _targetPosRef);

		/// @brief ���[�v���[�V�����̃^�[�Q�b�g�̍��W���Z�b�g
		/// @param _targetWorldPos �^�[�Q�b�g�̍��W
		void SetWarpTargetPos(const DXSimp::Vector3& _targetPos);

		/// @brief �g�����X�t�H�[�����擾����
		/// @return �v���C���[�̃g�����X�t�H�[��
		Transform* GetPlayerTransform();

		/// @brief �A�j���[�V�����R���g���[���|�[�l���g���擾
		/// @return �R���|�[�l���g
		CP_Animation* GetAnimation();

		void ImGuiDebug() override;
	private:
		/// @brief ���[�v���[�V�����ɂ��X�V����
		void WarpMotionUpdate();

		/// @brief ���̃��[�v���[�V�����ɓ��邩�ڍs���邩�m�F����
		/// @param _animRatio ���̃A�j���[�V��������
		void CheckTransNextWarp(float _animRatio);

		/// @brief ���[�v���[�V�����̃^�[�Q�b�g�̍��W���Z�b�g
		/// @param _targetWorldPos �^�[�Q�b�g�̍��W
		void CalcWarpDistance(const DXSimp::Vector3& _targetWorldPos);

		/// @brief �u���C�N�I���m�b�N���ł��邩�擾����
		/// @return �u���C�N�I���m�b�N�ł��邩�H
		bool GetCanBreakEndKdnock();

		// ���[�v���[�V�����Ɋւ���ImGui�ҏW
		void ImGuiWarpDebug();

		// ���[�v���[�V�����֘A�̃Z�[�u
		json SaveWarpParameters();

		// ���[�v���[�V�����֘A�̃��[�h
		void LoadWarpParameters(const json& _warpData);
	public:
		// �p�����[�^
		/// @brief �ړ����x�̃A�j���[�V�����p�����[�^��
		static constexpr auto SPEED_ANIMPARAM_NAME{ "speed" };
		/// @brief �U���g���K�[�̃A�j���[�V�����p�����[�^��
		static constexpr auto ATTACKTRIGGER_ANIMPARAM_NAME{ "attackTrigger" };
		/// @brief �U���g���K�[�̃A�j���[�V�����p�����[�^��
		static constexpr auto DAMAGETRIGGER_ANIMPARAM_NAME{ "damageTrigger" };
		// ���A�^�b�N����Ƃ��ɌĂяo���ϐ�
		static constexpr auto REATTACK_PARAMNAME{ "isReAttack" };

		// �A�j���[�V����
		/// @brief �ҋ@��Ԃ̃A�j���[�V������
		static constexpr auto IDLE_ANIM_NAME{ "Idle" };
		/// @brief �����Ԃ̃A�j���[�V������
		static constexpr auto RUN_ANIM_NAME{ "Move" };
	};
}
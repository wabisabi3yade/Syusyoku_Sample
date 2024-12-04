#pragma once
#include "EnemyActState_Base.h"
#include "CP_Animation.h"
#include "CP_RigidBody.h"

class BossActionController;

/// @brief �{�X�̃X�e�[�g�m�[�h���
class BossActState_Base : public EnemyActState_Base
{
protected:
	/// @brief  ���[�v���[�V�����ŕK�v�ȃp�����[�^
	struct WarpMotionParam
	{
		/// @brief ���[�v���[�V�����ňړ����銄��(XZ����)
		AnimationCurve horiMovementCurve;

		/// @brief ���[�v���[�V�����ňړ����銄��(Y����)
		AnimationCurve vertMovementCurve;

		/// @brief �ړ�����XZ�ő勗��(0�����Ȃ琧���Ȃ��Ƃ���)
		float maxMovementXZ{ -10.0f };

		/// @brief �ړ�����Y�ő勗��(0�����Ȃ琧���Ȃ��Ƃ���)
		float maxMovementY{ -10.0f };

		/// @brief �J�n���銄��
		float beginAnimRatio{ 0.0f };

		/// @brief �I�����銄��
		float endAnimRatio{ 0.0f };

		/// @brief Y�ړ������邩�H
		bool isUseVertical{ false };
	};

public:
	// �{�X�̃X�e�[�g�^�C�v
	enum class BossState
	{
		// �ړ�
		Idle,
		Walk,
		Run,

		// �U�镑��
		Damage_Small,	// ���_���[�W

		// �U��
		CombAttack1 = 100,	// �R���r�l�[�V�����U��
		JumpAttack,	// �W�����v�U��

		// �Ō�
		None
	};

private:
	/// @brief ���[�v���[�V�����̃p�����[�^�S
	std::vector<WarpMotionParam> warpMotionParams;

	/// @brief ���[�v���[�V�����̃^�[�Q�b�g��̃|�C���^
	const DirectX::SimpleMath::Vector3* pWarpTargetPos;

	/// @brief ���[�v���[�V�����ňړ�����Ƃ��̃^�[�Q�b�g���W�Ƃ̋���
	DirectX::SimpleMath::Vector3 disToWarpTargePos;

	/// @brief �Ō�̃��[�v���[�V�����ŋ��߂��J�[�u�l
	float lastWarpCurveValue;

	/// @brief �{�X�̃X�e�[�g
	BossState stateType;

	/// @brief �S�Ăŉ��񃏁[�v�����邩�H
	u_int allWarpSteps;

	/// @brief ���݂̃��[�v��
	u_int curWarpStep;

	/// @brief ���[�v���[�V���������邩�H
	bool isUseWarpMotion;

	/// @brief ���[�v���[�V���������H
	bool isWarpMoving;
protected:
	/// @brief �A�N�V�����R���g���[���[
	BossActionController* pActionController;
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

	/// @brief �_���[�W������
	void OnDamage();

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
protected:
	/// @brief ��Ԑ؂�ւ��J�n����
	virtual void OnStartBehavior() {}

	/// @brief �X�V����
	virtual void UpdateBehavior() {}

	/// @brief ��Ԑ؂�ւ��I������
	virtual void OnEndBehavior() {}

	/// @brief �J�ڐ؂�ւ��m�F
	virtual void TransitionCheckUpdate() {}

	/// @brief �J�ڂ���
	/// @param _nextState ���̑J��
	/// @param _isForce�@�����؂�ւ�
	void ChangeState(BossState _nextState, bool _isForce = false);

	/// @brief ���[�v���[�V�����̃^�[�Q�b�g�̍��W���Z�b�g
	/// @param _targetWorldPos �^�[�Q�b�g�̍��W
	void CalcWarpDistance(const DirectX::SimpleMath::Vector3& _targetWorldPos);

	/// @brief ���[�v���[�V�����̃^�[�Q�b�g�̍��W�̎Q�Ƃ��Z�b�g
	/// @param _targetWorldPos �^�[�Q�b�g�̍��W�̎Q��
	void SetWarpTargetPosReference(const DirectX::SimpleMath::Vector3& _targetPosRef);

	/// @brief �g�����X�t�H�[�����擾����
	/// @return �{�X�̃g�����X�t�H�[��
	Transform& GetBossTransform();

	/// @brief �g�����X�t�H�[�����擾����
	/// @return �v���C���[�̃g�����X�t�H�[��
	Transform& GetPlayerTransform();

	/// @brief �A�j���[�V�����R���g���[���|�[�l���g���擾
	/// @return �R���|�[�l���g
	CP_Animation* GetAnimation();

	/// @brief Rb���擾
	/// @return Rb�R���|�[�l���g
	CP_RigidBody* GetRB();

	/// @brief �o�ߎ��Ԃ��擾����
	/// @return �o�ߎ���
	float DeltaTime() const;

	void ImGuiDebug() override;
private:
	/// @brief ���[�v���[�V�����ɂ��X�V����
	void WarpMotionUpdate();

	/// @brief ���̃��[�v���[�V�����ɓ��邩�ڍs���邩�m�F����
	/// @param _animRatio ���̃A�j���[�V��������
	void CheckTransNextWarp(float _animRatio);

	// ���[�v���[�V�����Ɋւ���ImGui�ҏW
	void ImGuiWarpDebug();

	// ���[�v���[�V�����֘A�̃Z�[�u
	nlohmann::json SaveWarpParameters();

	// ���[�v���[�V�����֘A�̃��[�h
	void LoadWarpParameters(const nlohmann::json& _warpData);
protected:
	// �p�����[�^
	/// @brief �ړ����x�̃A�j���[�V�����p�����[�^��
	static constexpr auto SPEED_ANIMPARAM_NAME{ "speed" };
	/// @brief �U���g���K�[�̃A�j���[�V�����p�����[�^��
	static constexpr auto ATTACKTRIGGER_ANIMPARAM_NAME{ "attackTrigger" };
	/// @brief �U���g���K�[�̃A�j���[�V�����p�����[�^��
	static constexpr auto DAMAGETRIGGER_ANIMPARAM_NAME{ "damageTrigger" };

	// �A�j���[�V����
	/// @brief �ҋ@��Ԃ̃A�j���[�V������
	static constexpr auto IDLE_ANIM_NAME{ "Idle" };
	/// @brief �����Ԃ̃A�j���[�V������
	static constexpr auto RUN_ANIM_NAME{ "Move" };
};

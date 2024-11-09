#pragma once
#include "EnemyActState_Base.h"

class BossActionController;

/// @brief �{�X�̃X�e�[�g�m�[�h���
class BossActState_Base : public EnemyActState_Base
{
public:
	// �{�X�̃X�e�[�g�^�C�v
	enum class BossState
	{
		// �ړ�
		Idle,
		Walk,
		Run,

		// �U��
		Attack1 = 100,
		Attack2,

		// �Ō�
		None
	};

private:
	/// @brief �{�X�̃X�e�[�g
	BossState stateType;


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

	nlohmann::json Save();
	void Load(const nlohmann::json& _data);
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
	void ChangeState(BossState _nextState);

	/// @brief �g�����X�t�H�[�����擾����
	/// @return �{�X�̃g�����X�t�H�[��
	Transform& GetBossTransform();

	/// @brief �g�����X�t�H�[�����擾����
	/// @return �v���C���[�̃g�����X�t�H�[��
	Transform& GetPlayerTransform();

	void ImGuiSetting() override;

protected:
	/// @brief �ړ����x�̃A�j���[�V�����p�����[�^��
	static constexpr auto SPEED_ANIMPARAM_NAME{ "speed" };
	/// @brief �U���g���K�[�̃A�j���[�V�����p�����[�^��
	static constexpr auto ATTACKTRIGGER_ANIMPARAM_NAME{ "attackTrigger" };
	/// @brief �ҋ@��Ԃ̃A�j���[�V������
	static constexpr auto RUN_ANIM_NAME{ "Move" };
};

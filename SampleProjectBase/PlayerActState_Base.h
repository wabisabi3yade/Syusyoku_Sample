#pragma once

#include "CharacterActState_Base.h"
#include "GameInput.h"
#include "CP_Camera.h"
#include "CP_RigidBody.h"

class PlayerActionController;

/// @brief �v���C���[�̍s���X�e�[�g���N���X
class PlayerActState_Base : public CharacterActState_Base
{
public:
	// �v���C���[�̍s�����
	enum class PlayerState
	{
		// �ҋ@
		Idle,
		
		// �ړ�
		Move = 10,
		TargetMove,
		Jump,
		Rolling,

		// �n��R���r�l�[�V�����U��
		Attack11 = 20,
		Attack12,
		Attack13,
		Attack14,

		// �n��K�E�U��
		SpecialAtkHi = 30,
		SpecialAtkLow,

		// �Ō�
		None = 99
	};

#ifdef EDIT
	// �X�e�[�g�ꗗ�̕�����
	inline static const std::vector<std::string> playerStateNameList =
	{
		// �ҋ@
		"Idle",

		// �ړ�
		"Move",
		"TargetMove",
		"Jump",
		"Rolling"

		// �n��R���r�l�[�V�����U��
		"Attack11",
		"Attack12",
		"Attack13",
		"Attack14",

		// �n��K�E�U��
		"SpecialAtkHi",
		"SpecialAtkLow",

		// �Ō�
		"None"
	};
#endif // EDIT

private:
	/// @brief ���̍s���N���X�̃X�e�[�g�^�C�v
	PlayerState stateType;

	/// @brief ���͊��Ԓ��ɍU����������Ă�����?
	bool isAttackInput;

protected:
	/// @brief �v���C���[
	PlayerActionController* pActionController;

	/// @brief �Q�[�����̓N���X
	static GameInput* pPlayerInput;

	/// @brief �J�����N���X
	static CP_Camera* pCamera;
public:
	PlayerActState_Base();
	virtual ~PlayerActState_Base() {}

	/// @brief ����������
	/// @param _stateType�@���
	/// @param _actController�@�v���C���[�R���|�[�l���g
	/// @param _changeObserver �X�e�[�g�J�ڃI�u�U�[�o�[
	void Init(PlayerState _stateType, PlayerActionController& _actController);

	/// @brief �J�n�����Ăяo��
	void OnStart() override;

	/// @brief �X�V�����Ăяo��
	void Update() override;

	/// @brief  �I�������Ăяo��
	void OnEnd() override;

	// �X�e�[�g�^�C�v���擾
	PlayerState GetActStateType() const;

	/// @brief �Z�[�u����
	/// @return �Z�[�u�f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����f�[�^ 
	void Load(const nlohmann::json& _data) override;
protected:
	/// @brief �e��Ԃ̊J�n����
	virtual void OnStartBehavior() {};

	/// @brief �X�V����
	virtual void UpdateBehavior() {};

	/// @brief �e��Ԃ̏I������
	virtual void OnEndBehavior() {};

	/// @brief �X�e�[�g�J�ڏ����̃`�F�b�N����
	virtual void TransitionCheckUpdate();

	/// @brief ���͂̃t���O���N���A
	void InputFlagClear();

	/// @brief ���͊m�F�̍X�V
	void CheckInputUpdate();

	/// @brief �ǂ̃A�N�V�����ɂ����ʂ���J��
	void CommmonCheckTransition();

	/// @brief ��Ԃ�J�ڂ���
	/// @param _changeSate �J�ڐ�̏��
	void ChangeState(PlayerState _nextState);

	/// @brief RigidBody���擾
	/// @return RigidBody
	CP_RigidBody& GetRB();

	/// @brief ��t���擾
	/// @return ��t
	float DeltaTime() const;

	/// @brief ��t��i�߂鑬�x���擾
	/// @return ��t�i�߂鑬�x
	float GetDeltaSpeed() const;

	/// @brief �R���g���[���[�̍��X�e�B�b�N�̓��͂��擾
	/// @return ���X�e�B�b�N�̓���
	DirectX::SimpleMath::Vector2 GetInputLeftStick() const;

	/// @brief ���[�����O�ł��邩�擾����
	/// @return ���[�����O�ł��邩�H
	bool GetCanRolling() const;

	/// @brief �U���ł��邩�擾����
	/// @return �U���ł��邩�H
	bool GetCanAttack() const;

	/// @brief ImGui����
	void ImGuiDebug() override {}

	/// @brief ImGui�ɂ��R���{�{�b�N�X
	/// @param _caption �L���v�V����
	/// @param _currentState ���݂̃X�e�[�g
	/// @return �ύX�������H
	static bool ImGuiComboPlayerState(const std::string& _caption, PlayerState& _currentState);

private:


	/// @brief �L�����Z���ɍ��킹�ăA�N�V�������s�����m�F
	/// @return �X�e�[�g�������ŕύX�������H
	bool CheckCanCancelTransition();

	bool CheckCanRolling();

protected:
	// �A�j���[�V�����R���g���[�����̃v���C���[��
	constexpr static auto SPEEDRATIO_PARAMNAME = "speed";	// �ړ����x
	constexpr static auto MOVEAXIS_X_PARAMNAME = "axisX";	// �ړ����x
	constexpr static auto MOVEAXIS_Y_PARAMNAME = "axisY";	// �U���g���K�[
};


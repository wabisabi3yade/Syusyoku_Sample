#pragma once

#include "CharacterActState_Base.h"
#include "GameInput.h"
#include "CP_Camera.h"
#include "CP_RigidBody.h"
#include "CP_Animation.h"

class CP_Player;
class PlayerActionController;

/// @brief �v���C���[�̍s���X�e�[�g���N���X
class PlayerActState_Base : public CharacterActState_Base
{
public:
	/// @brief  �v���C���[�̍s�����
	enum class PlayerState
	{
		// �ҋ@
		Idle,

		// �ړ�
		Move,
		TargetMove,
		Jump,
		Rolling,
		Damage_S,	// �_���[�W���̂�����
		Damage_L,	// �_���[�W��̂�����
		Guard,	// �K�[�h�J�n
		Guard_Parry,	// �p���B����

		// �n��R���r�l�[�V�����U��
		Attack11 = 20,
		Attack12,
		Attack13,
		Attack14,

		// �n��K�E�U��
		SpecialAtkHi = 30,
		SpecialAtkLow,
		SpecialAtkGuard,	// �K�[�h�p���B���̍U��

		// �Ō�
		None = 99
	};

	/// @brief �A�N�V�����̏ꏊ����
	enum class ActPlaceElement
	{
		Ground,
		Air
	};

	/// @brief �v���C���[���猩�����͂̕���
	enum class InputVector
	{
		Forward,	// �O����
		Back	// ���
	};

#ifdef EDIT
	// �X�e�[�g�ꗗ�̕�����
	inline static const std::vector<std::string> playerCombAtkList =
	{
		// �n��R���r�l�[�V�����U��
		"Attack11",
		"Attack12",
		"Attack13",
		"Attack14",
	};
#endif // EDIT

private:
	/// @brief ���̍s���N���X�̃X�e�[�g�^�C�v
	PlayerState stateType;

	/// @brief �L�����Z�����ČJ��o�����
	PlayerState cancelPlayState;

	/// @brief �s���̏ꏊ����
	ActPlaceElement placeElement;

	/// @brief �^�[�Q�b�g������Ƃ��̉�]���x
	float targetLookRotateSpeed;

	/// @brief ���͊��Ԓ��ɍU����������Ă�����?
	bool isAttackInput;

	/// @brief �^�[�Q�b�g���ɓG������s���ɂ��邩�H
	bool isTargetLookAtEnemy;

protected:
	/// @brief �v���C���[
	PlayerActionController* pActionController;

	/// @brief �Q�[�����̓N���X
	static GameInput* pPlayerInput;
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
	void ParameterClear();

	/// @brief ���͊m�F�̍X�V
	virtual void CheckInputUpdate();

	/// @brief ��Ԃ�J�ڂ���
	/// @param _changeSate �J�ڐ�̏��
	void ChangeState(PlayerState _nextState);

	/// @brief ���̍s���̓^�[�Q�b�g������̂��Z�b�g
	/// @param _isLook ����̂��H
	void SetTargetAtEnemy(bool _isLook);

	/// @brief ���G�ɂ��邩�H
	/// @param _isInvicible ���G
	void SetInvicible(bool _isInvicible);

	/// @brief RigidBody���擾
	/// @return RigidBody
	CP_RigidBody& GetRB();

	/// @brief �v���C���[�̃g�����X�t�H�[�����擾
	/// @return �g�����X�t�H�[��
	Transform& GetTransform();

	/// @brief �A�j���[�V�������擾����
	/// @return �A�j���[�V����
	CP_Animation* GetAnimation();

	/// @brief �v���C���[�R���|�[�l���g���擾����
	/// @return �v���C���[�R���|�[�l���g
	CP_Player& GetPlayer();

	/// @brief ��t���擾
	/// @return ��t
	float DeltaTime() const;

	/// @brief ��t��i�߂鑬�x���擾
	/// @return ��t�i�߂鑬�x
	float GetDeltaSpeed() const;

	/// @brief �R���g���[���[�̍��X�e�B�b�N�̓��͂��擾
	/// @return ���X�e�B�b�N�̓���
	DirectX::SimpleMath::Vector2 GetInputLeftStick() const;

	/// @brief ���̕����ɓ��͂ł��Ă��邩�m�F����
	/// @param _checkVector �m�F����������
	/// @return �ł��Ă��邩�H
	bool IsInputVector(InputVector _checkVector);

	/// @brief ���[�����O���͂ł��Ă��邩�H
	/// @return ���[�����O�ł��邩�H
	bool IsRollingInput() const;

	/// @brief �K�E�Z���͂ł��Ă��邩�H
	/// @param _inputVecter �����̓���
	/// @return  �K�E�Z�ł��邩�H
	bool IsSpecialAtkInput(InputVector _inputVecter);

	/// @brief �U���ł��邩�擾����
	/// @return �U���ł��邩�H
	bool GetCanCombAttack();

	/// @brief ImGui����
	void ImGuiDebug() override;

	/// @brief ImGui�ɂ��R���{�{�b�N�X
	/// @param _caption �L���v�V����
	/// @param _currentState ���݂̃X�e�[�g
	/// @return �ύX�������H
	static bool ImGuiComboAttack(const std::string& _caption, PlayerState& _currentState);

private:
	/// @brief �^�[�Q�b�g�̕���������
	void UpdateTargetLook();

	/// @brief �L�����Z���ɍ��킹�ăA�N�V������ύX���邩�`�F�b�N
	void CancelTransitionUpdate();
protected:
	// �A�j���[�V�����R���g���[�����̃v���C���[��
	constexpr static auto SPEEDRATIO_PARAMNAME = "speed";	// �ړ����x
	constexpr static auto MOVEAXIS_X_PARAMNAME = "axisX";	// X�ړ�
	constexpr static auto MOVEAXIS_Y_PARAMNAME = "axisY";	// Y�ړ�
	// �U���g���K�[
	constexpr static auto DAMAGETRIGGER_PARAMNAME = "damageTrigger";

	// �A�j���[�V������
	constexpr static auto IDLE_ANIM_NAME = "Idle";	// �ҋ@���
};


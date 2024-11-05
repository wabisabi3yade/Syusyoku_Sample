#pragma once

#include "StateMachine.h"
#include "GameInput.h"
#include "IObserever.h"
#include "CP_Camera.h"

#include "AnimationController.h"


class CP_Animation;
class CP_Player;

/// @brief �v���C���[�̍s���X�e�[�g���N���X
class PlayerActState_Base : public HashiTaku::StateNode_AnimationBase , public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
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
	using StateChangeObserver = HashiTaku::IObserver<int>;
	using StateChangeSubject = HashiTaku::Subject<int>;

	/// @brief �X�e�[�g�J�ڂ��邱�Ƃ�ʒm������T�u�W�F�N�g
	std::unique_ptr<StateChangeSubject> changeStateSubject;

	/// @brief ���̍s���N���X�̃X�e�[�g�^�C�v
	PlayerState stateType;
protected:
	/// @brief �A�j���[�V�����Ǘ�
	CP_Animation* pAnimation;

	/// @brief �v���C���[
	CP_Player* pPlayer;

	/// @brief �Q�[�����̓N���X
	GameInput* pPlayerInput;

	/// @brief �J�����N���X
	static CP_Camera* pCamera;
public:
	PlayerActState_Base();
	virtual ~PlayerActState_Base() {}

	/// @brief ����������
	/// @param _stateType�@���
	/// @param _player�@�v���C���[�R���|�[�l���g
	/// @param _changeObserver �X�e�[�g�J�ڃI�u�U�[�o�[
	void Init(PlayerState _stateType, CP_Player& _player, StateChangeObserver& _changeObserver);

	/// @brief �J�n�����Ăяo��
	void OnStart() override;

	/// @brief �X�V�����Ăяo��
	void Update() override;

	/// @brief  �I�������Ăяo��
	void OnEnd() override;

	// �A�j���[�V�������Z�b�g����
	void SetAnimation(CP_Animation& _pAnimation);

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
	virtual void TransitionCheckUpdate() {}

	/// @brief ��Ԃ�J�ڂ���
	/// @param _changeSate �J�ڐ�̏��
	void ChangeState(PlayerState _nextState);

	/// @brief �R���g���[���[�̍��X�e�B�b�N�̓��͂��擾
	/// @return ���X�e�B�b�N�̓���
	DirectX::SimpleMath::Vector2 GetInputLeftStick() const;

	/// @brief ImGui����
	virtual void ImGuiSetting() {}

	/// @brief ImGui�ɂ��R���{�{�b�N�X
	/// @param _caption �L���v�V����
	/// @param _currentState ���݂̃X�e�[�g
	/// @return �ύX�������H
	static bool ImGuiComboPlayerState(const std::string& _caption, PlayerState& _currentState);

protected:
	// �A�j���[�V�����R���g���[�����̃v���C���[��
	constexpr static auto SPEEDRATIO_PARAMNAME = "speed";	// �ړ����x
	constexpr static auto MOVEAXIS_X_PARAMNAME = "axisX";	// �ړ����x
	constexpr static auto MOVEAXIS_Y_PARAMNAME = "axisY";	// �U���g���K�[
	constexpr static auto CANCEL_PARAMNAME = "canCancel";	// �L�����Z���ł��邩�H
};


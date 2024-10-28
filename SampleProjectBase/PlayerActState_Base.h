#pragma once

#include "StateMachine.h"
#include "GameInput.h"
#include "IObserever.h"
#include "CP_Camera.h"

#include "AnimationController.h"


class CP_Animation;
class GameObject;

/// @brief �v���C���[�̍s���X�e�[�g���N���X
class PlayerActState_Base : public HashiTaku::StateNode_Base , public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
public:
	// �v���C���[�̍s�����
	enum class StateType
	{
		// �ҋ@
		Idle,	// 0
		
		// �ړ�
		Move = 11,	// 11
		TargetMove,	// 12
		Jump,	// 13

		// �U��
		NormalAttack1 = 20,	// 20

		// �Ō�
		None
	};

private:
	using StateChangeObserver = HashiTaku::IObserver<int>;
	using StateChangeSubject = HashiTaku::Subject<int>;

	/// @brief �X�e�[�g�J�ڂ��邱�Ƃ�ʒm������T�u�W�F�N�g
	std::unique_ptr<StateChangeSubject> changeStateSubject;

	/// @brief ���̍s���N���X�̃X�e�[�g�^�C�v
	StateType stateType;
protected:
	/// @brief �A�j���[�V�����Ǘ�
	CP_Animation* pAnimation;

	/// @brief  �v���C���[�I�u�W�F�N�g
	GameObject* pPlayerObject;

	/// @brief �Q�[�����̓N���X
	GameInput* pPlayerInput;

	/// @brief �J�����N���X
	static CP_Camera* pCamera;
public:
	PlayerActState_Base();
	virtual ~PlayerActState_Base() {}

	/// @brief ����������
	/// @param _stateType�@���
	/// @param _gameObject�@�ړ�����I�u�W�F�N�g 
	/// @param _changeObserver �X�e�[�g�J�ڃI�u�U�[�o�[
	void Init(StateType _stateType, GameObject& _gameObject, StateChangeObserver& _changeObserver);

	/// @brief �J�n�����Ăяo��
	void OnStartCall();

	/// @brief �X�V�����Ăяo��
	void UpdateCall();

	/// @brief  �I�������Ăяo��
	void OnEndCall();

	// �A�j���[�V�������Z�b�g����
	void SetAnimation(CP_Animation& _pAnimation);

	/// @brief �A�N�V������ԗ񋓌^�𕶎���ɕϊ�
	/// @param _stateType ������ɕϊ��������A�N�V�������
	/// @return ������
	static std::string StateTypeToStr(StateType _stateType);

	// �X�e�[�g�^�C�v���擾
	StateType GetActStateType() const;

	/// @brief �Z�[�u����
	/// @return �Z�[�u�f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����f�[�^ 
	void Load(const nlohmann::json& _data) override;
protected:
	/// @brief �e��Ԃ̊J�n����
	void OnStart() override {};

	/// @brief �X�V����
	void Update() override {};

	/// @brief �e��Ԃ̏I������
	void OnEnd() override {};

	/// @brief ��Ԃ�J�ڂ���
	/// @param _changeSate �J�ڐ�̏��
	void ChangeState(StateType _nextState);

	/// @brief	���͒l��Ԃ�
	DirectX::SimpleMath::Vector2 InputValue();

	/// @brief ImGui����
	virtual void ImGuiSetting() {}

protected:
	// �A�j���[�V�����R���g���[�����̃v���C���[��
	constexpr static auto SPEEDRATIO_PARAMNAME = "speed";	// �ړ����x
	constexpr static auto ATTACKTRIGGER_PARAMNAME = "attackTrigger";	// �U���g���K�[
	constexpr static auto MOVEAXIS_X_PARAMNAME = "AxisX";	// �ړ����x
	constexpr static auto MOVEAXIS_Y_PARAMNAME = "AxisY";	// �U���g���K�[
};


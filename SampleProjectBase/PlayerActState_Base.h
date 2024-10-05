#pragma once

// �Q�[��������
#include "GameInput.h"
#include "IObserever.h"

#include "AnimationController.h"

class CP_Animation;
class GameObject;

/// @brief �v���C���[�̍s���X�e�[�g���N���X
class PlayerActState_Base : public HashiTaku::IImGuiUser
{
public:
	// �v���C���[�̍s�����
	enum class StateType
	{
		Move,
		Jump,
		Attack,
		MaxNum
	};

private:
	/// @brief �X�e�[�g�J�ڂ��邱�Ƃ�ʒm������T�u�W�F�N�g
	std::unique_ptr<HashiTaku::Subject<int>> changeStateSubject;

	/// @brief ���̍s���N���X�̃X�e�[�g�^�C�v
	StateType stateType;
protected:
	/// @brief �A�j���[�V�����Ǘ�
	CP_Animation* pAnimation;

	/// @brief  �v���C���[�I�u�W�F�N�g
	GameObject* pPlayerObject;
public:
	PlayerActState_Base(StateType _stateType);
	virtual ~PlayerActState_Base() {}

	/// @brief ����������
	/// @param _gameObject�@�ړ�����I�u�W�F�N�g 
	/// @param _changeObserver �X�e�[�g�J�ڃI�u�U�[�o�[
	void Init(GameObject& _gameObject, HashiTaku::IObserver<int>& _changeObserver);

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

protected:
	/// @brief �e��Ԃ̊J�n����
	virtual void OnStart() = 0;

	/// @brief �X�V����
	virtual void Update() = 0;

	/// @brief �e��Ԃ̏I������
	virtual void OnEnd() = 0;

	/// @brief ��Ԃ�J�ڂ���
	/// @param _changeSate �J�ڐ�̏��
	void ChangeState(StateType _changeState);

	/// @brief ImGui����
	virtual void ImGuiSetting() {}

protected:
	// �A�j���[�V�����R���g���[�����̃v���C���[��
	constexpr static auto SPEEDRATIO_PARAMNAME = "speed";	// �ړ����x
	constexpr static auto ATTACKTRIGGER_PARAMNAME = "attackTrigger";	// �U���g���K�[



};


#pragma once
#include "StateMachine.h"
#include "ChangeAnimObserver.h"

class CharacterChangeAnimObserver;
class CP_Animation;
class CP_Character;

/// @brief �L�����N�^�[�i�v���C���[�A�G�Ȃ�)�̃A�N�V�����R���g���[���[
class CharacterActionController : public HashiTaku::StateMachine_Base<int>, public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
	/// @brief �A�j���[�V�����ύX�����Ƃ��̃I�u�U�[�o�[
	std::unique_ptr<CharacterChangeAnimObserver> pChangeAnimObserver;

protected:
	/// @brief �A�j���[�V�����R���|�[�l���g	
	CP_Animation* pAnimation;

	/// @brief �L�����N�^�[�R���|�[�l���g
	CP_Character* pCharacter;
public:
	CharacterActionController(CP_Character& _character, const std::string& _stateMachineName);
	~CharacterActionController() {}

	/// @brief ����������
	/// @param _animationController �A�j���[�V�����R���|�[�l���g
	virtual void Init(CP_Animation& _animation);

	/// @brief �A�j���[�V�����ύX�I�u�U�[�o�[���擾
	/// @return �A�j���[�V�����ύX�I�u�U�[�o�[
	CharacterChangeAnimObserver& GetChangeAnimObserver();

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override {};

protected:
	virtual void ImGuiSetting() = 0;
};

/// @brief �L�����N�^�[���A�j���[�V�����J�ڂ����Ƃ��̃I�u�U�[�o�[
class CharacterChangeAnimObserver : public HashiTaku::ChangeAnimObserver
{
	/// @brief �A�N�V�����R���g���[���[
	CharacterActionController* pActionController;

public:
	/// @brief �R���X�g���N�^
	/// @param _playerActCon �A�N�V�����R���g���[���[
	/// @param _observerName �I�u�U�[�o�[��
	CharacterChangeAnimObserver(CharacterActionController& _playerActCon,
		const std::string& _observerName);
	~CharacterChangeAnimObserver() {}

	/// @brief �ʒm�������Ƃ��̏���
	void ObserverUpdate(const HashiTaku::ChangeAnimationInfo& _value);
};
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

	/// @brief �L�����N�^�[�R���|�[�l���g
	CP_Character* pCharacter;

#ifdef EDIT
	bool isDebugDisplay;
#endif // EDIT
protected:
	/// @brief �A�j���[�V�����R���|�[�l���g	
	CP_Animation* pAnimation;
public:
	CharacterActionController(CP_Character& _character, const std::string& _stateMachineName);
	~CharacterActionController() {}

	/// @brief ����������
	/// @param _animationController �A�j���[�V�����R���|�[�l���g
	virtual void Init(CP_Animation& _animation);

	/// @brief �A�j���[�V�����ύX�I�u�U�[�o�[���擾
	/// @return �A�j���[�V�����ύX�I�u�U�[�o�[
	CharacterChangeAnimObserver& GetChangeAnimObserver();

	/// @brief �A�j���[�V�������擾
	/// @return �A�j���[�V�����R���|�[�l���g
	CP_Animation& GetAnimation();

	/// @brief �f�o�b�O�`����s��
	void DebugDisplay();

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override {};

protected:
	/// @brief �L�����N�^�[�R���|�[�l���g���擾
	/// @return �L�����N�^�[�R���|�[�l���g
	CP_Character& GetCharacter();

	virtual void ImGuiSetting();
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
#pragma once
#include "StateMachine.h"

/// @brief �L�����N�^�̃A�N�V�����X�e�[�g���
class CharacterActState_Base : public HashiTaku::StateNode_Base, public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
public:
	CharacterActState_Base() {}
	virtual ~CharacterActState_Base() {}

	/// @brief �A�j���[�V�����I�����̍s��
	/// @param _fromAnimNodeName �J�ڌ��̃A�j���[�V�����m�[�h��
	/// @param _toAnimNodeName �J�ڐ�̃A�j���[�V�����m�[�h��
	virtual void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
	{}

	/// @brief �f�o�b�O���ŕ`�悵�����Ƃ��̏���
	virtual void DebugDisplay() {}

	virtual nlohmann::json Save()  = 0;
	virtual void Load(const nlohmann::json& _data) = 0;
protected:
	virtual void ImGuiSetting() = 0;
};


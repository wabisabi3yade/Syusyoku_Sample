#pragma once
#include "StateMachine.h"

#ifdef EDIT
#include "Geometory.h"
#endif // EDIT

namespace HashiTaku
{
	/// @brief �L�����N�^�̃A�N�V�����X�e�[�g���
	class CharacterActState_Base :
		public StateNode_Base,
		public IImGuiUser,
		public ISaveLoad
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

		virtual nlohmann::json Save() = 0;
		virtual void Load(const nlohmann::json& _data) = 0;
	protected:
		virtual void ImGuiDebug() = 0;
	};
}
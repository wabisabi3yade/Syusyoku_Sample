#pragma once
#include "BossActState_Base.h"

namespace HashiTaku
{
	/// @brief �{�X�_���[�W���炢�X�e�[�g
	class BossDamageState : public BossActState_Base
	{

	public:
		BossDamageState();
		virtual ~BossDamageState() {}

	protected:
		/// @brief �J�n����
		void OnStartBehavior() override;

		/// @brief �A�j���[�V�����I�����̍s��
		/// @param _fromAnimNodeName �J�ڌ��̃A�j���[�V�����m�[�h��
		/// @param _toAnimNodeName �J�ڐ�̃A�j���[�V�����m�[�h��
		void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName) override;

	private:
		/// @brief �_���[�W���v���C���[�Ɍ�����
		void LookPlayer();
	};
}
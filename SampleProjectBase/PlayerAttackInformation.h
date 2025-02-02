#pragma once
#include "AttackInformation.h"

namespace HashiTaku
{
	/// @brief �v���C���[�̍U�����
	class PlayerAttackInformation : public AttackInformation
	{
		/// @brief �u���C�N�l
		float breakValue;
	public:
		/// @brief �R���X�g���N�^
		/// @param _pAttacker �U����
		PlayerAttackInformation(IAttacker* _pAttacker);
		~PlayerAttackInformation() {}

		/// @brief �u���C�N�l���擾����
		/// @return �u���C�N�l
		float GetBreakValue() const;

		json Save() override;
		void Load(const json& _data) override;
	private:
		void ImGuiDebug() override;
	};
}



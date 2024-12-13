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
		PlayerAttackInformation();
		~PlayerAttackInformation() {}

		/// @brief �u���C�N�l���擾����
		/// @return �u���C�N�l
		float GetBreakValue() const;

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	private:
		void ImGuiDebug() override;
	};
}



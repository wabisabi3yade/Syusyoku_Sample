#pragma once
#include "AttackInformation.h"

namespace HashiTaku
{
	/// @brief �G���̍U�����
	class BossAttackInformation : public AttackInformation
	{
		/// @brief �p���B���ɒǉ�����u���C�N�l
		float addBreakValueOnParry;
	public:
		BossAttackInformation();
		~BossAttackInformation() {}

		/// @brief �p���B���ɉ��Z����u���C�N�l���擾
		/// @return �p���B���ɉ��Z����u���C�N�l
		float GetBreakValueOnParry() const;

		json Save() override;
		void Load(const json& _data) override;

	private:
		void ImGuiDebug() override;
	};
}


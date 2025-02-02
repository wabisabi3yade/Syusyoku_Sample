#pragma once
#include "AttackInformation.h"
#include "IAttacker_AcceptParry.h"

namespace HashiTaku
{
	/// @brief �G���̍U�����
	class BossAttackInformation : public AttackInformation
	{
		/// @brief �p���B���ɒǉ�����u���C�N�l
		float addBreakValueOnParry;

		/// @brief �p���B���ꂽ����߂��悤�ɂ��邩�H
		bool isStunOnParry;
	public:
		BossAttackInformation(IAttacker* _pAttacker);
		~BossAttackInformation() {}

		/// @brief �p���B���ꂽ���Ƃ�ʒm����
		/// @param _acceptInfo �p���B����̏��
		void NotifyAcceptParry(const AcceptParryInfo& _acceptInfo);

		/// @brief �p���B���ɉ��Z����u���C�N�l���擾
		/// @return �p���B���ɉ��Z����u���C�N�l
		float GetBreakValueOnParry() const;

		/// @brief �p���B���ꂽ����߂����擾
		/// @return �p���B���ꂽ����߂����H
		bool GetIsStunOnParry() const;

		json Save() override;
		void Load(const json& _data) override;

	private:
		IAttacker_AcceptParry* CastAcceptParry();

		void ImGuiDebug() override;
	};
}


#pragma once
#include "CP_Character.h"

class CP_Enemy : public HashiTaku::CP_Character
{

public:
	CP_Enemy();
	~CP_Enemy() {}

private:
	/// @brief �e�N���X�̃_���[�W���󂯂��Ƃ��̏���
		/// @param _attackInfo �U�����
	virtual void OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo);

	/// @brief �e�N���X�̃_���[�W���󂯂��Ƃ��̏���
	/// @param _attackInfo �U�����
	virtual void OnDeathBehavior();
};


#pragma once
#include "CP_Character.h"

class CP_Enemy : public CP_Character
{
	/// @brief �G�̖���
	std::string enemyName;

public:
	CP_Enemy();
	virtual ~CP_Enemy() {}

	/// @brief �G�����擾
	/// @return �G��
	const std::string& GetEnemyName() const;
private:
	/// @brief �e�N���X�̃_���[�W���󂯂��Ƃ��̏���
	/// @param _attackInfo �U�����
	void OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo) override;

	/// @brief �e�N���X�̃_���[�W���󂯂��Ƃ��̏���
	/// @param _attackInfo �U�����
	void OnDeathBehavior() override;

protected:
	void Awake() override;
	void OnDestroy() override;

	void SetEnemyName(const std::string& _enemyName);
};


#pragma once
#include "Component.h"

/// @brief ����R���|�[�l���g
class CP_Weapon : public Component
{
	/// @brief �U������R���W����
	bool isAttackCollision;

public:
	CP_Weapon();
	~CP_Weapon() {}

	void SetAttackInfo();

	/// @brief ����̍U������R���W����
	/// @param _isAttackCollision 
	void SetIsAttackCollision(bool _isAttackCollision);
};


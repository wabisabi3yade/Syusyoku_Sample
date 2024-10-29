#pragma once
#include "Component.h"

/// @brief 武器コンポーネント
class CP_Weapon : public Component
{
	/// @brief 攻撃判定コリジョン
	bool isAttackCollision;

public:
	CP_Weapon();
	~CP_Weapon() {}

	void SetAttackInfo();

	/// @brief 武器の攻撃判定コリジョン
	/// @param _isAttackCollision 
	void SetIsAttackCollision(bool _isAttackCollision);
};


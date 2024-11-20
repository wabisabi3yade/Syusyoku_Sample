#pragma once

namespace HashiTaku
{
	class IDamageable;
}

// 攻撃をするインターフェース
class IAttacker
{

public:
	IAttacker() {}
	virtual ~IAttacker() {}

	/// @brief 攻撃処理
	/// @param _damager 攻撃与える対象
	virtual void OnAttack(HashiTaku::IDamageable& _damager) = 0;
};


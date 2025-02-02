#pragma once
#include "AttackInformation.h"

namespace HashiTaku
{
	/// @brief 攻撃するオブジェクトのインターフェース
	class IAttacker
	{

	public:
		IAttacker() = default;
		virtual ~IAttacker() = default;

		/// @brief 攻撃者のワールド座標を取得する
		/// @return 所有者のワールド座標
		virtual const DXSimp::Vector3& GetAttackerWorldPos() const = 0;

		/// @brief 攻撃ヒットさせたときに起こす処理
		virtual void OnAttacking(const AttackInformation& _atkInfo) = 0;
	};

}


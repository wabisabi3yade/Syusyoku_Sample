#pragma once
#include "IAttacker.h"
#include "IParryAccepter.h"

namespace HashiTaku
{
	/// @brief 攻撃するオブジェクト。パリィされたときに処理を起こすインターフェース
	class IAttacker_AcceptParry : public IAttacker, public IParryAccepter
	{

	public:
		IAttacker_AcceptParry() = default;
		virtual ~IAttacker_AcceptParry() = default;
	};
}



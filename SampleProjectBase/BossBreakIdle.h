#pragma once
#include "BossActState_Base.h"

namespace HashiTaku
{
	/// @brief ブレイク時の行動
	class BossBreakIdle : public BossActState_Base
	{

	public:
		BossBreakIdle() {}
		~BossBreakIdle() {}

		/// @brief 遷移更新
		void TransitionCheckUpdate() override;
	};
}


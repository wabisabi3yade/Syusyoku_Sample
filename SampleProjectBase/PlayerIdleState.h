#pragma once
#include "PlayerGroundState.h"

namespace HashiTaku
{
	class PlayerIdleState : public PlayerGroundState
	{
	public:
		PlayerIdleState();
		~PlayerIdleState() {}

	private:
		void OnStartBehavior() override;
	};
}
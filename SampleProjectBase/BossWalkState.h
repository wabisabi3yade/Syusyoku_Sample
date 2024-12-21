#pragma once
#include "BossGroundMove.h"

namespace HashiTaku
{
	/// @brief �{�X����
	class BossWalkState : public BossGroundMove
	{
	public:
		BossWalkState();
		~BossWalkState() {}

		json Save() override;
		void Load(const json& _data) override;
	private:
		void TransitionCheckUpdate() override;

		void ImGuiDebug() override;
	};
}
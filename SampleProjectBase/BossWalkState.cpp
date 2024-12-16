#include "pch.h"
#include "BossWalkState.h"
#include "BossActionController.h"

namespace HashiTaku
{
	BossWalkState::BossWalkState()
	{
	}

	nlohmann::json BossWalkState::Save()
	{
		auto data = BossGroundMove::Save();
		return data;
	}

	void BossWalkState::Load(const nlohmann::json& _data)
	{
		BossGroundMove::Load(_data);
	}

	void BossWalkState::TransitionCheckUpdate()
	{
		BossGroundMove::TransitionCheckUpdate();

		// ˆê’è‹——£—£‚ê‚½‚ç‰“‹——£UŒ‚
		DXSimp::Vector3 distance =
			GetPlayerTransform().GetPosition() - GetBossTransform().GetPosition();
		distance.y = 0.0f;
		if (distance.Length() > pActionController->GetActDistanceLength(BossActionController::ActDistance::Far))
		{
			ChangeState(BossState::Idle);
		}
	}

	void BossWalkState::ImGuiDebug()
	{
		BossGroundMove::ImGuiDebug();
	}
}
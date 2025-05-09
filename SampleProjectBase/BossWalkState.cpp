#include "pch.h"
#include "BossWalkState.h"
#include "BossActionController.h"

namespace HashiTaku
{
	BossWalkState::BossWalkState()
	{
	}

	json BossWalkState::Save()
	{
		auto data = BossGroundMove::Save();
		return data;
	}

	void BossWalkState::Load(const json& _data)
	{
		BossGroundMove::Load(_data);
	}

	void BossWalkState::TransitionCheckUpdate()
	{
		BossGroundMove::TransitionCheckUpdate();

		Transform* pPlayerTransform = GetPlayerTransform();
		if (!pPlayerTransform) return;

		// ˆê’è‹——£—£‚ê‚½‚ç‰“‹——£UŒ‚
		DXSimp::Vector3 distance =
			pPlayerTransform->GetPosition() - GetMyTransform().GetPosition();
		distance.y = 0.0f;
		if (distance.Length() > GetBossActionController().GetActDistanceLength(BossActionController::ActDistance::Far))
		{
			ChangeState(BossState::Idle);
		}
	}

	void BossWalkState::ImGuiDebug()
	{
		BossGroundMove::ImGuiDebug();
	}
}
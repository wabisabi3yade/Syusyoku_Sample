#include "pch.h"
#include "PlayerTargetMove.h"
#include "GameInput.h"
#include "GameObject.h"
#include "Geometory.h"

namespace HashiTaku
{
	const float BLEND_OFFSET(0.5f);	// ブレンドを0.0f～1.0fに収める

	PlayerTargetMove::PlayerTargetMove()
		: rotateSpeed(180.0f)
	{
		// ターゲットの方向に向く
		SetTargetAtEnemy(true);
	}

	json PlayerTargetMove::Save()
	{
		auto data = PlayerMoveState::Save();

		return data;
	}

	void PlayerTargetMove::Load(const json& _data)
	{
		using namespace HashiTaku;
		PlayerMoveState::Load(_data);
	}

	void PlayerTargetMove::UpdateBehavior()
	{
		Move();

		ApplyBlendAnim();

		ApplyRootMotion();
	}

	void PlayerTargetMove::OnEndBehavior()
	{
		PlayerMoveState::OnEndBehavior();
	}

	void PlayerTargetMove::TransitionCheckUpdate()
	{
		PlayerMoveState::TransitionCheckUpdate();
	}

	void PlayerTargetMove::ApplyBlendAnim()
	{
		GameObject& playerObj = GetPlayer().GetGameObject();

		// アニメーションのブレンド割合をセット
		DXSimp::Vector2 playerFwd =
		{
			playerObj.GetTransform().Forward().x,
			playerObj.GetTransform().Forward().z
		};
		playerFwd.Normalize();

		DXSimp::Vector2 playerRight =
		{
			playerObj.GetTransform().Right().x,
			playerObj.GetTransform().Right().z
		};
		playerRight.Normalize();

		DXSimp::Vector2 moveVector2D =
		{
			moveVector.x,
			moveVector.z
		};

		DXSimp::Vector2 moveAxis = DXSimp::Vector2::One * BLEND_OFFSET;
		if (moveVector2D.Length() > Mathf::epsilon)
		{

			// 進行方向と右ベクトルとの角度を求める
			float dotVR = moveVector2D.Dot(playerRight);
			dotVR = std::clamp(dotVR, -1.0f, 1.0f);	// 偶に1.0を超えることがあるので
			float ang = acosf(dotVR);

			float dotVF = moveVector2D.Dot(playerFwd);
			if (dotVF < 0.0f)
				ang *= -1;

			moveAxis.x = cosf(ang) * 0.5f + BLEND_OFFSET;
			moveAxis.y = sinf(ang) * 0.5f + BLEND_OFFSET;
		}

		pActionController->SetAnimationFloat(MOVEAXIS_X_PARAMNAME, moveAxis.x);
		pActionController->SetAnimationFloat(MOVEAXIS_Y_PARAMNAME, moveAxis.y);
	}

	void PlayerTargetMove::ApplyRootMotion()
	{
		CP_Animation* pAnimation = pActionController->GetAnimation();
		if (!pAnimation) return;

		float rootMotion = pAnimation->GetMotionPosSpeedPerSec().Length();

		if (rootMotion > Mathf::epsilon)
		{
			float animPlaySpeed = currentSpeed / rootMotion;

			pAnimation->SetCurNodePlayerSpeed(animPlaySpeed);
		}

	}

	void PlayerTargetMove::ImGuiDebug()
	{
		PlayerMoveState::ImGuiDebug();
	}
}
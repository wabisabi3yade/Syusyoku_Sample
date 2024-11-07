#include "pch.h"
#include "PlayerTargetMove.h"
#include "GameInput.h"
#include "GameObject.h"
#include "Geometory.h"

const float BLEND_OFFSET(0.5f);	// ブレンドを0.0f～1.0fに収める

PlayerTargetMove::PlayerTargetMove()
{
}

nlohmann::json PlayerTargetMove::Save()
{
	auto data = PlayerMoveState::Save();

	return data;
}

void PlayerTargetMove::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;
	PlayerMoveState::Load(_data);
}

void PlayerTargetMove::OnStartBehavior()
{

}

void PlayerTargetMove::UpdateBehavior()
{
	Move();

	ApplyBlendAnim();

	ApplyRootMotion();
}

void PlayerTargetMove::OnEndBehavior()
{
}

void PlayerTargetMove::TransitionCheckUpdate()
{
	using enum GameInput::ButtonType;

	if (!IsRunning())
		ChangeState(PlayerState::Idle);

	else if (!pActionController->GetIsTargeting())
		ChangeState(PlayerState::Move);

	//else if (pPlayerInput->GetButtonDown(Player_Attack) && GetInputLeftStick().y > 0.8f)
	//	ChangeState(PlayerState::SpecialAtkHi);

	else if (pPlayerInput->GetButtonDown(Player_Attack))
		ChangeState(PlayerState::Attack11);
}

void PlayerTargetMove::ApplyBlendAnim()
{
	using namespace DirectX::SimpleMath;

	GameObject& playerObj = pActionController->GetPlayer().GetGameObject();

	// アニメーションのブレンド割合をセット
	Vector2 playerFwd =
	{
		playerObj.GetTransform().Forward().x,
		playerObj.GetTransform().Forward().z
	};
	playerFwd.Normalize();

	Vector2 playerRight =
	{
		playerObj.GetTransform().Right().x,
		playerObj.GetTransform().Right().z
	};
	playerRight.Normalize();

	Vector2 moveVector2D =
	{
		moveVector.x,
		moveVector.z
	};

	Vector2 moveAxis = Vector2::One * BLEND_OFFSET;
	if (moveVector2D.Length() > Mathf::epsilon)
	{
		// 進行方向と右ベクトルとの角度を求める
		float dotVR = moveVector2D.Dot(playerRight);
		float ang = acosf(dotVR);

		float dotVF = moveVector2D.Dot(playerFwd);
		if (dotVF < 0.0f)
			ang *= -1;

		moveAxis.x = cos(ang) * 0.5f + BLEND_OFFSET;
		moveAxis.y = sin(ang) * 0.5f + BLEND_OFFSET;	
	}

	pAnimation->SetFloat(MOVEAXIS_X_PARAMNAME, moveAxis.x);
	pAnimation->SetFloat(MOVEAXIS_Y_PARAMNAME, moveAxis.y);
}

void PlayerTargetMove::ApplyRootMotion()
{
	float rootMotion = pAnimation->GetMotionPosSpeedPerSec().Length();

	if (rootMotion > Mathf::epsilon)
	{
		float animPlaySpeed = currentSpeed / rootMotion;

		pAnimation->SetCurNodePlayerSpeed(animPlaySpeed);
	}

}

void PlayerTargetMove::ImGuiSetting()
{
	PlayerMoveState::ImGuiSetting();
}

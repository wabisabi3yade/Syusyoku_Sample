#include "pch.h"
#include "PlayerTargetMove.h"
#include "GameInput.h"
#include "GameObject.h"
#include "Geometory.h"

constexpr float IDLE_ANIM_PLAYSPEED(1.0f);

PlayerTargetMove::PlayerTargetMove()
	:currentSpeed(0.0f), maxSpeed(10.0f), acceleration(40.0f), decadeSpeedTimes(0.98f)
{
}

nlohmann::json PlayerTargetMove::Save()
{
	auto data = PlayerActState_Base::Save();

	data["maxSpeed"] = maxSpeed;
	data["acceleration"] = acceleration;
	data["decade"] = decadeSpeedTimes;

	return data;
}

void PlayerTargetMove::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;
	PlayerActState_Base::Load(_data);

	LoadJsonFloat("maxSpeed", maxSpeed, _data);
	LoadJsonFloat("acceleration", acceleration, _data);
	LoadJsonFloat("decade", decadeSpeedTimes, _data);
}

void PlayerTargetMove::OnStart()
{
}

void PlayerTargetMove::Update()
{
	Move();

	if (GameInput::GetInstance()->GetButtonUp(GameInput::ButtonType::Player_RockOn))
	{
		ChangeState(StateType::Move);
	}
}

void PlayerTargetMove::OnEnd()
{
}

void PlayerTargetMove::Move()
{
	using namespace DirectX::SimpleMath;

	float deltaTime = MainApplication::DeltaTime();

	Vector3 camForwardVec = pCamera->GetTransform().Forward();
	Vector3 camRightVec = pCamera->GetTransform().Right();
	Vector2 input = InputValue();

	float mag = input.Length();
	HASHI_DEBUG_LOG(std::to_string(mag));

	// 移動方向・移動量決定
	moveVector = camRightVec * input.x;
	moveVector += camForwardVec * input.y;
	moveVector.y = 0.0f;
	moveVector.Normalize();

	Vector3 moveSpeed = moveVector * mag * maxSpeed;
	currentSpeed = moveSpeed.Length();

	// 移動
	Vector3 pos = pPlayerObject->GetTransform().GetPosition();
	pos += moveVector * maxSpeed * MainApplication::DeltaTime();
	pPlayerObject->GetTransform().SetPosition(pos);

	// アニメーションのブレンド割合をセット
	Vector2 playerFwd =
	{
		pPlayerObject->GetTransform().Forward().x,
		pPlayerObject->GetTransform().Forward().z
	};
	playerFwd.Normalize();

	Vector2 playerRight =
	{
		pPlayerObject->GetTransform().Right().x,
		pPlayerObject->GetTransform().Right().z
	};
	playerRight.Normalize();

	Vector2 moveVector2D =
	{
		moveVector.x,
		moveVector.z
	};
	moveVector.Normalize();

	// ルートモーションと移動速度から移動速度の再生速度を調整する
	if (IsRunning())
	{
		float dotFwd = moveVector2D.Dot(playerFwd);
		float dotRight = moveVector2D.Dot(playerRight);

		float degFwd = acosf(dotFwd);

		if (dotRight < 0.0f)
			degFwd *= -1;

		constexpr float DEG_MINUS(90.0f * Mathf::degToRad);

		Vector2 dirBlend = { cosf(degFwd - DEG_MINUS), sinf(degFwd + DEG_MINUS) };
		dirBlend = (dirBlend + Vector2::One) * 0.5f;

		pAnimation->SetFloat(MOVEAXIS_X_PARAMNAME, dirBlend.x);
		pAnimation->SetFloat(MOVEAXIS_Y_PARAMNAME, dirBlend.y);

		//float rootMotion = abs(pAnimation->GetMotionPosSpeedPerSec().z);

		//if (rootMotion < Mathf::epsilon)
		//	rootMotion = Mathf::epsilon;

		//float animPlaySpeed = currentSpeed / rootMotion;

		//pAnimation->SetCurPlayerSpeed(animPlaySpeed);
	}
	else
	{
		pAnimation->SetFloat(MOVEAXIS_X_PARAMNAME, 0.5f);
		pAnimation->SetFloat(MOVEAXIS_Y_PARAMNAME, 0.5f);
		pAnimation->SetCurPlayerSpeed(IDLE_ANIM_PLAYSPEED);
	}

	pAnimation->SetFloat(SPEEDRATIO_PARAMNAME, currentSpeed / maxSpeed);
}


bool PlayerTargetMove::IsRunning()
{
	return currentSpeed >= Mathf::epsilon;
}

void PlayerTargetMove::ImGuiSetting()
{
	PlayerActState_Base::ImGuiSetting();

	if (!ImGuiMethod::TreeNode("TargetMove")) return;

	std::string text = TO_UTF8("speed") + std::to_string(currentSpeed);
	ImGui::Text(text.c_str());
	ImGui::DragFloat("maxSpeed", &maxSpeed, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("acceleration", &acceleration, 0.1f);
	ImGui::DragFloat("decadeTimes", &decadeSpeedTimes, 0.01f, 0.0f, 1.0f);

	ImGui::TreePop();
}

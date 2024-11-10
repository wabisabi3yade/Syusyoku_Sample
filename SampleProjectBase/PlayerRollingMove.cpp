#include "pch.h"
#include "PlayerRollingMove.h"
#include "CP_Player.h"

constexpr auto ROLLING_ANIMNODE_NAME("RollingMove");	// ローリングのアニメーションノード名

PlayerRollingMove::PlayerRollingMove()
	: rollingDistance(5.0f), invicibleTime(0.4f)
{
	pSpeedCurve = std::make_unique<AnimationCurve>();
}

void PlayerRollingMove::OnStartBehavior()
{
	PlayerActState_Base::OnStartBehavior();

	// 入力の向きに即時に向ける
	TurnInputVec();

	// 回避開始座標を取得
	Transform& transform = pActionController->GetPlayer().GetTransform();
	rollingStartPos = transform.GetPosition();

	elapsedTime = 0.0f;

	// 無敵
	pActionController->GetPlayer().SetIsInvicible(true);

	// アニメーションの反映
	pAnimation->SetTrigger(ROLLING_ANIMPARAM);
}

void PlayerRollingMove::UpdateBehavior()
{
	using namespace DirectX::SimpleMath;
	PlayerActState_Base::UpdateBehavior();

	// 移動
	Move();

	// 無敵時間の更新
	UpdateInvicible();
}

void PlayerRollingMove::OnEndBehavior()
{
	pActionController->GetPlayer().SetIsInvicible(false);
}

void PlayerRollingMove::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
{
	// アニメーションの遷移先が待機状態なら待機に戻す
	if (_fromAnimNodeName == ROLLING_ANIMNODE_NAME)
		ChangeState(PlayerState::Idle);
}

void PlayerRollingMove::TurnInputVec()
{
	using namespace DirectX::SimpleMath;
	Transform& transform = pActionController->GetPlayer().GetTransform();

	// 向く方向を求める
	Vector3 camForwardVec = pCamera->GetTransform().Forward();
	Vector3 camRightVec = pCamera->GetTransform().Right();
	Vector2 input = GetInputLeftStick();

	Vector3 turnVec;
	turnVec = camRightVec * input.x;
	turnVec += camForwardVec * input.y;
	turnVec.y = 0.0f;
	turnVec.Normalize();

	// オブジェクトを向ける
	Quaternion targetRotation = Quat::RotateToVector(turnVec);
	transform.SetRotation(targetRotation);
}

void PlayerRollingMove::Move()
{
	using namespace DirectX::SimpleMath;

	Transform& transform = pActionController->GetPlayer().GetTransform();
	float deltaTime = MainApplication::DeltaTime();

	// アニメーションの再生割合から進む距離を求める
	float animPlayRatio = pAnimation->GetCurrentPlayRatio();
	float moveDistance = pSpeedCurve->GetValue(animPlayRatio) * rollingDistance;

	// 前進方向に移動する
	Vector3 pos = transform.GetPosition();
	pos = rollingStartPos + transform.Forward() * moveDistance;
	transform.SetPosition(pos);
}

void PlayerRollingMove::UpdateInvicible()
{
	// 無敵時間内か見る
	elapsedTime += MainApplication::DeltaTime();
	CP_Player& player = pActionController->GetPlayer();

	if (player.GetIsInvicible() && elapsedTime > invicibleTime)
		pActionController->GetPlayer().SetIsInvicible(false);
}

void PlayerRollingMove::ImGuiDebug()
{
	PlayerActState_Base::ImGuiDebug();

	ImGui::DragFloat("Distance", &rollingDistance, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("InvicibleTime", &invicibleTime, 0.01f, 0.0f, 5.0f);
	pSpeedCurve->ImGuiCall();
}

nlohmann::json PlayerRollingMove::Save()
{
	auto data = PlayerActState_Base::Save();

	data["rollingDistance"] = rollingDistance;
	data["invicibleTime"] = invicibleTime;
	data["speedCurve"] = pSpeedCurve->Save();

	return data;
}

void PlayerRollingMove::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;

	PlayerActState_Base::Load(_data);

	LoadJsonFloat("rollingDistance", rollingDistance, _data);
	LoadJsonFloat("invicibleTime", invicibleTime, _data);

	nlohmann::json speedCurveData;
	if (LoadJsonData("speedCurve", speedCurveData, _data))
	{
		pSpeedCurve->Load(speedCurveData);
	}
}

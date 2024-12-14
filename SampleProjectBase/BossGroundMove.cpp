#include "pch.h"
#include "BossGroundMove.h"
#include "GameObject.h"

BossGroundMove::BossGroundMove()
	: maxSpeed(9.0f), acceleration(4.0f), currentSpeed(0.0f), rotateSpeed(120.0f)
{
}

nlohmann::json BossGroundMove::Save()
{
	auto data = BossActState_Base::Save();

	data["moveMaxSpeed"] = maxSpeed;
	data["moveAcceleration"] = acceleration;
	data["moveRotateSpeed"] = rotateSpeed;

	return data;
}

void BossGroundMove::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;

	BossActState_Base::Load(_data);

	LoadJsonFloat("moveMaxSpeed", maxSpeed, _data);
	LoadJsonFloat("moveAcceleration", acceleration, _data);
	LoadJsonFloat("moveRotateSpeed", rotateSpeed, _data);
}

void BossGroundMove::UpdateBehavior()
{
	// 向きを回転する
	Rotation();

	// 移動
	Move();
}

void BossGroundMove::OnEndBehavior()
{
	// ブレンドアニメーションの速度を渡す
	pActionController->GetAnimation()->SetFloat(SPEED_ANIMPARAM_NAME, 0.0f);

	GetRB().SetVelocity({ 0.0f, 0.0f ,0.0f });
}

void BossGroundMove::TransitionCheckUpdate()
{
	// 近距離範囲に入っているか
	if (CheckNearTransition())
	{
		pActionController->ChangeState(BossState::Idle);
	}

}

void BossGroundMove::Rotation()
{
	using namespace DirectX::SimpleMath;
	float deltaTime = DeltaTime();

	Transform& myTransform = GetBossTransform();
	Transform& playerTrans = GetPlayerTransform();

	// プレイヤーへの方向ベクトルを求める
	Vector3 targetVec = playerTrans.GetPosition() - myTransform.GetPosition();
	targetVec.y = 0.0f;
	targetVec.Normalize();

	// ベクトルからクォータニオンを生成
	Quaternion targetRotation = Quat::RotateToVector(targetVec);

	// 回転させる
	Quaternion rot = myTransform.GetRotation();
	rot = Quaternion::Slerp(rot, targetRotation, rotateSpeed * deltaTime);
	myTransform.SetRotation(rot);
}

void BossGroundMove::Move()
{
	using namespace DirectX::SimpleMath;

	float deltaTime = DeltaTime();
	Transform& myTransform = pActionController->GetBoss().GetTransform();

	// 加速
	currentSpeed += acceleration * deltaTime;
	currentSpeed = std::min(currentSpeed, maxSpeed);

	// 正面方向に移動
	Vector3 movement = myTransform.Forward() * currentSpeed;

	// 移動する(y座標は反映させない)
	auto& pRb = GetRB();
	movement.y = pRb.GetVelocity().y;
	pRb.SetVelocity(movement);

	CP_Animation* pAnimation = GetAnimation();
	if (!pAnimation) return;

	if (maxSpeed < Mathf::epsilon) // 0徐算禁止
		maxSpeed = Mathf::epsilon;

	pAnimation->SetFloat(SPEED_ANIMPARAM_NAME, currentSpeed / maxSpeed);

	float rootMotion = abs(pAnimation->GetMotionPosSpeedPerSec().z);

	if (rootMotion > Mathf::epsilon)
	{
		float animPlaySpeed = currentSpeed / rootMotion;

		pAnimation->SetCurNodePlayerSpeed(animPlaySpeed);
	}
}

bool BossGroundMove::CheckNearTransition()
{
	using namespace DirectX::SimpleMath;

	// プレイヤーとの距離
	Vector3 playerToDis = GetPlayerTransform().GetPosition() -
		GetBossTransform().GetPosition();
	float distanceLen = playerToDis.Length();

	// 範囲外なら
	float shortDis =
		pActionController->GetActDistanceLength(BossActionController::ActDistance::Short);
	if (distanceLen > shortDis) return false;

	return true;
}

void BossGroundMove::ImGuiDebug()
{
	ImGui::DragFloat("MaxSpeed", &maxSpeed, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("Acceleration", &acceleration, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("RotateSpeed", &rotateSpeed, 0.1f, 0.0f, 1000.0f);
}


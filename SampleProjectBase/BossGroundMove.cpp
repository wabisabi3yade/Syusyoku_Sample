#include "pch.h"
#include "BossGroundMove.h"
#include "GameObject.h"
#include "Geometory.h"

BossGroundMove::BossGroundMove()
	: maxSpeed(9.0f), acceleration(4.0f), currentSpeed(0.0f), rotateSpeed(120.0f), transNearDistance(6.0f)
{
}

void BossGroundMove::DebugDisplay()
{
#ifdef EDIT
	using namespace DirectX::SimpleMath;
	Geometory::SetColor(Color(1, 1, 1, 0.5f));
	Geometory::SetPosition(GetBossTransform().GetPosition());
	Geometory::SetScale(Vector3::One * transNearDistance * 2);
	Geometory::DrawSphere();
#endif
}

nlohmann::json BossGroundMove::Save()
{
	auto data = BossGroundState::Save();

	data["moveMaxSpeed"] = maxSpeed;
	data["moveAcceleration"] = acceleration;
	data["moveRotateSpeed"] = rotateSpeed;
	data["transNearDistance"] = transNearDistance;

	return data;
}

void BossGroundMove::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;

	BossGroundState::Load(_data);

	LoadJsonFloat("moveMaxSpeed", maxSpeed, _data);
	LoadJsonFloat("moveAcceleration", acceleration, _data);
	LoadJsonFloat("moveRotateSpeed", rotateSpeed, _data);
	LoadJsonFloat("transNearDistance", transNearDistance, _data);
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
	pActionController->GetAnimation().SetFloat(SPEED_ANIMPARAM_NAME, 0.0f);
}

void BossGroundMove::TransitionCheckUpdate()
{
	// 近距離範囲に入っているか
	if (CheckNearTransition())
	{
		pActionController->ChangeState(BossState::Attack1);
	}

}

void BossGroundMove::Rotation()
{
	using namespace DirectX::SimpleMath;

	float deltaTime = MainApplication::DeltaTime();

	Transform& myTransform = GetBossTransform();
	Transform& playerTrans = GetPlayerTransform();

	Vector3 targetVec = playerTrans.GetPosition() - myTransform.GetPosition();
	targetVec.y = 0.0f;
	targetVec.Normalize();

	// 正面と目標ベクトルとの角度の差分を求める
	float diffAng = acosf(targetVec.Dot(myTransform.Forward()));
	diffAng = std::max(diffAng, Mathf::smallValue);
	float deltaAngle = rotateSpeed * Mathf::degToRad * MainApplication::DeltaTime();

	// 回転速度が差分を超えたら
	if (diffAng < deltaAngle)
	{
		deltaAngle = diffAng;

	}

	// どっち方向に回転させるか
	float dotRight = targetVec.Dot(myTransform.Right());
	if (dotRight < 0.0f)
		deltaAngle *= -1;

	// 回転させる
	Quaternion deltaRot = Quaternion::CreateFromAxisAngle(Vec3::Up, deltaAngle);
	Quaternion rot = myTransform.GetRotation();
	rot = Quat::Multiply(rot, deltaRot);
	myTransform.SetRotation(rot);
}

void BossGroundMove::Move()
{
	using namespace DirectX::SimpleMath;

	float deltaTime = MainApplication::DeltaTime();
	Transform& myTransform = pActionController->GetBoss().GetTransform();

	// 加速
	currentSpeed += acceleration * deltaTime;
	currentSpeed = std::min(currentSpeed, maxSpeed);

	// 正面方向に移動
	Vector3 movement = myTransform.Forward() * currentSpeed * deltaTime;

	// 座標反映
	Vector3 pos = myTransform.GetPosition();
	pos += movement;
	myTransform.SetPosition(pos);

	// ブレンドアニメーションの速度を渡す
	pActionController->GetAnimation().SetFloat(SPEED_ANIMPARAM_NAME, currentSpeed / maxSpeed);
}

bool BossGroundMove::CheckNearTransition()
{
	using namespace DirectX::SimpleMath;

	// プレイヤーとの距離
	Vector3 playerToDis = GetPlayerTransform().GetPosition() -
		GetBossTransform().GetPosition();
	float distanceLen = playerToDis.Length();

	// 範囲外なら
	if (distanceLen > transNearDistance) return false;

	return true;
}

void BossGroundMove::ImGuiSetting()
{
	ImGui::DragFloat("MaxSpeed", &maxSpeed, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("Acceleration", &acceleration, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("RotateSpeed", &rotateSpeed, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("TransitionNear", &transNearDistance, 0.1f, 0.0f, 1000.0f);

}


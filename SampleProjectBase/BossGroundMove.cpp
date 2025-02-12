#include "pch.h"
#include "BossGroundMove.h"
#include "GameObject.h"

namespace HashiTaku
{
	BossGroundMove::BossGroundMove()
		: maxSpeed(9.0f), acceleration(4.0f), currentSpeed(0.0f), rotateSpeed(120.0f)
	{
	}

	json BossGroundMove::Save()
	{
		auto data = BossActState_Base::Save();

		data["moveMaxSpeed"] = maxSpeed;
		data["moveAcceleration"] = acceleration;
		data["moveRotateSpeed"] = rotateSpeed;

		return data;
	}

	void BossGroundMove::Load(const json& _data)
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
		GetActionController().SetAnimationFloat(SPEED_ANIMPARAM_NAME, 0.0f);

		GetActionController().SetVelocity({0.0f, 0.0f ,0.0f});
	}

	void BossGroundMove::TransitionCheckUpdate()
	{
		// 近距離範囲に入っているか
		if (CheckNearTransition())
		{
			GetBossActionController().ChangeState(BossState::Idle);
		}

	}

	void BossGroundMove::Rotation()
	{
		using namespace DXSimp;
		float deltaTime = DeltaTime();

		Transform& myTransform = GetMyTransform();
		Transform* playerTrans = GetPlayerTransform();
#ifdef EDIT
		if (!playerTrans) return;
#endif // EDIT


		// プレイヤーへの方向ベクトルを求める
		Vector3 targetVec = playerTrans->GetPosition() - myTransform.GetPosition();
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
		using namespace DXSimp;

		float deltaTime = DeltaTime();
		Transform& myTransform = GetMyTransform();

		// 加速
		currentSpeed += acceleration * deltaTime;
		currentSpeed = std::min(currentSpeed, maxSpeed);

		// 正面方向に移動
		Vector3 movement = myTransform.Forward() * currentSpeed;

		// 移動する(y座標は反映させない)
		BossActionController& bossActCon = GetBossActionController();
		DXSimp::Vector3 curVelocity;
		bossActCon.GetVelocity(curVelocity);
		movement.y = curVelocity.y;
		bossActCon.SetVelocity(movement);

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
		using namespace DXSimp;

		Transform* pPlayerTrans = GetPlayerTransform();
		BossActionController& bossActCon = GetBossActionController();

		if (!pPlayerTrans) return false;

		// プレイヤーとの距離
		Vector3 playerToDis = pPlayerTrans->GetPosition() -
			GetMyTransform().GetPosition();
		float distanceLen = playerToDis.Length();

		// 範囲外なら
		float shortDis =
			bossActCon .GetActDistanceLength(BossActionController::ActDistance::Short);
		if (distanceLen > shortDis) return false;

		return true;
	}

	void BossGroundMove::ImGuiDebug()
	{
		ImGui::DragFloat("MaxSpeed", &maxSpeed, 0.1f, 0.0f, 1000.0f);
		ImGui::DragFloat("Acceleration", &acceleration, 0.1f, 0.0f, 1000.0f);
		ImGui::DragFloat("RotateSpeed", &rotateSpeed, 0.1f, 0.0f, 1000.0f);
	}

}
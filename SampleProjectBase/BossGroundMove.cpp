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
		// ��������]����
		Rotation();

		// �ړ�
		Move();
	}

	void BossGroundMove::OnEndBehavior()
	{
		// �u�����h�A�j���[�V�����̑��x��n��
		GetActionController().SetAnimationFloat(SPEED_ANIMPARAM_NAME, 0.0f);

		GetActionController().SetVelocity({0.0f, 0.0f ,0.0f});
	}

	void BossGroundMove::TransitionCheckUpdate()
	{
		// �ߋ����͈͂ɓ����Ă��邩
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


		// �v���C���[�ւ̕����x�N�g�������߂�
		Vector3 targetVec = playerTrans->GetPosition() - myTransform.GetPosition();
		targetVec.y = 0.0f;
		targetVec.Normalize();

		// �x�N�g������N�H�[�^�j�I���𐶐�
		Quaternion targetRotation = Quat::RotateToVector(targetVec);

		// ��]������
		Quaternion rot = myTransform.GetRotation();
		rot = Quaternion::Slerp(rot, targetRotation, rotateSpeed * deltaTime);
		myTransform.SetRotation(rot);
	}

	void BossGroundMove::Move()
	{
		using namespace DXSimp;

		float deltaTime = DeltaTime();
		Transform& myTransform = GetMyTransform();

		// ����
		currentSpeed += acceleration * deltaTime;
		currentSpeed = std::min(currentSpeed, maxSpeed);

		// ���ʕ����Ɉړ�
		Vector3 movement = myTransform.Forward() * currentSpeed;

		// �ړ�����(y���W�͔��f�����Ȃ�)
		BossActionController& bossActCon = GetBossActionController();
		DXSimp::Vector3 curVelocity;
		bossActCon.GetVelocity(curVelocity);
		movement.y = curVelocity.y;
		bossActCon.SetVelocity(movement);

		CP_Animation* pAnimation = GetAnimation();
		if (!pAnimation) return;

		if (maxSpeed < Mathf::epsilon) // 0���Z�֎~
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

		// �v���C���[�Ƃ̋���
		Vector3 playerToDis = pPlayerTrans->GetPosition() -
			GetMyTransform().GetPosition();
		float distanceLen = playerToDis.Length();

		// �͈͊O�Ȃ�
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
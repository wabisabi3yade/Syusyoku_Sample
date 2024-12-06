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
	// ��������]����
	Rotation();

	// �ړ�
	Move();
}

void BossGroundMove::OnEndBehavior()
{
	// �u�����h�A�j���[�V�����̑��x��n��
	pActionController->GetAnimation()->SetFloat(SPEED_ANIMPARAM_NAME, 0.0f);
}

void BossGroundMove::TransitionCheckUpdate()
{
	// �ߋ����͈͂ɓ����Ă��邩
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

	// �v���C���[�ւ̕����x�N�g�������߂�
	Vector3 targetVec = playerTrans.GetPosition() - myTransform.GetPosition();
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
	using namespace DirectX::SimpleMath;

	float deltaTime = DeltaTime();
	Transform& myTransform = pActionController->GetBoss().GetTransform();

	// ����
	currentSpeed += acceleration * deltaTime;
	currentSpeed = std::min(currentSpeed, maxSpeed);

	// ���ʕ����Ɉړ�
	Vector3 movement = myTransform.Forward() * currentSpeed;

	// �ړ�����(y���W�͔��f�����Ȃ�)
	auto* pRb = GetRB();
	movement.y = pRb->GetVelocity().y;
	pRb->SetVelocity(movement);

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
	using namespace DirectX::SimpleMath;

	// �v���C���[�Ƃ̋���
	Vector3 playerToDis = GetPlayerTransform().GetPosition() -
		GetBossTransform().GetPosition();
	float distanceLen = playerToDis.Length();

	// �͈͊O�Ȃ�
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


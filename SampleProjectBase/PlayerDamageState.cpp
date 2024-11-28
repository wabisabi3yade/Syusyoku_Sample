#include "pch.h"
#include "PlayerDamageState.h"

namespace DXSimp = DirectX::SimpleMath;

PlayerDamageState::PlayerDamageState() :
	maxKnockMoveSpeed(0.0f), prevAnimRatio(0.0f), isKnockMoving(false), isInvicible(false)
{
}

void PlayerDamageState::SetKnockVec(const DirectX::SimpleMath::Vector3& _knockVec)
{
	knockVector = _knockVec;
}

nlohmann::json PlayerDamageState::Save()
{
	auto data = PlayerActState_Base::Save();

	data["knockDis"] = maxKnockMoveSpeed;
	data["knockCurve"] = knockSpeedCurve.Save();

	return data;
}

void PlayerDamageState::Load(const nlohmann::json& _data)
{
	PlayerActState_Base::Load(_data);

	HashiTaku::LoadJsonFloat("knockDis", maxKnockMoveSpeed, _data);
	if (HashiTaku::IsJsonContains(_data, "knockCurve"))
	{
		knockSpeedCurve.Load(_data["knockCurve"]);
	}
}

void PlayerDamageState::OnStartBehavior()
{
	// �A�j���[�V�����ɒʒm
	GetAnimation()->SetTrigger(DAMAGETRIGGER_PARAMNAME);

	// �G�̕���������
	LookEnemy();

	// �̂�����ɖ��G�����邩
	SetInvicible(isInvicible);

	// ���Z�b�g
	prevAnimRatio = 0.0f;
	isKnockMoving = true;
}

void PlayerDamageState::UpdateBehavior()
{
	KnockMove();
}

void PlayerDamageState::OnEndBehavior()
{
	// ���G����
	SetInvicible(false);
}

void PlayerDamageState::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
{
	if (_toAnimNodeName == IDLE_ANIM_NAME)	// �ҋ@�ɕς������
		ChangeState(PlayerState::Idle);
}

void PlayerDamageState::KnockMove()
{
	if (!isKnockMoving) return;	// �m�b�N�ړ�������Ȃ��Ȃ�

	float animRatio = GetAnimation()->GetCurrentPlayRatio();

	// �O��̊������O�ɗ����Ȃ�
	if (prevAnimRatio > animRatio)
	{
		isKnockMoving = false;	// �ړ����I����
		GetRB().SetVelocity(DXSimp::Vector3::Zero);
		return;
	}

	// ���݂̐�����񂾋������擾
	float curKnockSpeed = knockSpeedCurve.GetValue(animRatio) * maxKnockMoveSpeed;

	// �ړ����x�����߂�(���Ɉړ�)
	DXSimp::Vector3 moveSpeed =
		-GetTransform().Forward() * curKnockSpeed;
	GetRB().SetVelocity(moveSpeed);

	prevAnimRatio = animRatio;
}

void PlayerDamageState::LookEnemy()
{
	// �G�̕���������
	knockVector *= -1;
	auto rot = Quat::RotateToVector(knockVector);
	GetTransform().SetRotation(rot);
}

void PlayerDamageState::ImGuiDebug()
{
	ImGui::Checkbox("Invicible", &isInvicible);
	ImGui::DragFloat("knockDistance", &maxKnockMoveSpeed, 0.1f, 0.0f, 1000.0f);
	knockSpeedCurve.ImGuiCall();
}

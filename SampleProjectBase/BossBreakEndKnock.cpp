#include "pch.h"
#include "BossBreakEndKnock.h"
#include "CP_Boss.h"

constexpr auto ANIMATION_NAME("BreakEndKnock");	// �u���C�N�I���̃m�b�N�A�j���[�V������

namespace DXSimp = HashiTaku::DXSimp;

BossBreakEndKnock::BossBreakEndKnock() :
	canBreakValue(10.0f),
	knockDistance(4.0f)
{
}

float BossBreakEndKnock::GetCanBreakValue() const
{
	return canBreakValue;
}

nlohmann::json BossBreakEndKnock::Save()
{
	auto data = BossDamageState::Save();
	data["canBreak"] = canBreakValue;
	data["knockDistance"] = knockDistance;
	return data;
}

void BossBreakEndKnock::Load(const nlohmann::json& _data)
{
	BossDamageState::Load(_data);

	HashiTaku::LoadJsonFloat("canBreak", canBreakValue, _data);
	HashiTaku::LoadJsonFloat("knockDistance", knockDistance, _data);
}

void BossBreakEndKnock::OnStartBehavior()
{
	BossDamageState::OnStartBehavior();

	// �m�b�N�̍��W�����߂ă��[�v���[�V�����ɓn��
	CalcKnockTargetPos();
}

void BossBreakEndKnock::CalcKnockTargetPos()
{
	// �u���C�N�l�����Z�b�g
	pActionController->GetBoss().SetBreakValue(0.0f);

	Transform& transform = GetBossTransform();
	DXSimp::Vector3 targetPos = 
		transform.GetPosition() - transform.Forward() * knockDistance;

	// ���[�v���[�V�����̋�����ݒ�
	SetWarpTargetPos(targetPos);
}

void BossBreakEndKnock::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
{
	// �A�j���[�V�����I������Ƒҋ@
	if (_fromAnimNodeName == ANIMATION_NAME)
	{
		ChangeState(BossState::Idle);
	}
}

void BossBreakEndKnock::ImGuiDebug()
{
	BossDamageState::ImGuiDebug();

	ImGui::DragFloat("CanBreakEnd", &canBreakValue, 0.01f, 0.0f, 100.0f);
	ImGui::DragFloat("KnockDis", &knockDistance, 0.01f, 0.0f, 100.0f);
}

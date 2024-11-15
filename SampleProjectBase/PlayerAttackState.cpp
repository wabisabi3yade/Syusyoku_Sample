#include "pch.h"
#include "PlayerAttackState.h"
#include "PlayerActionController.h"
#include "GameObject.h"

PlayerAttackState::PlayerAttackState()
	: nextCombAtkState(PlayerState::None), atkMoveSpeed(10.0f), lookRotateSpeed(0.0f), isMoveForward(false), isAttackCollisionBefore(false)
{
	pAttackInfo = std::make_unique<HashiTaku::AttackInformation>();

	// �J�[�u�����Z�b�g
	progressDistanceCurve.SetCurveName("AtkMoveSpd");
}

void PlayerAttackState::OnStartBehavior()
{
	// �U�������X�V
	UpdateAttackInfo();

	// �U���t���O�𗧂Ă�
	pActionController->SetAnimationTrigger(ATTACKTRIGGER_PARAMNAME);

	isAttackCollisionBefore = false;
}

void PlayerAttackState::UpdateBehavior()
{
	// �U�����ɑO�i����
	ForwardProgressMove();
}

void PlayerAttackState::OnEndBehavior()
{
}

void PlayerAttackState::TransitionCheckUpdate()
{
	// �U�����͂��ꂽ��X�e�[�g�J�ڂ���
	if (GetCanAttack())
		ChangeState(nextCombAtkState);

	PlayerActState_Base::TransitionCheckUpdate();
}

void PlayerAttackState::UpdateAttackInfo()
{
	pActionController->GetPlayer().SetAttackInfo(*pAttackInfo);
}

void PlayerAttackState::LookAtEnemy()
{
	

}

void PlayerAttackState::ImGuiDebug()
{
	ImGui::Checkbox("MoveFwd", &isMoveForward);
	ImGui::Text("AtkInfo");
	pAttackInfo->ImGuiCall();

	if (isMoveForward)
	{
		ImGui::DragFloat("AtkMoveSpeed", &atkMoveSpeed, 0.1f);
		progressDistanceCurve.ImGuiCall();
	}
}

void PlayerAttackState::ForwardProgressMove()
{
	using namespace DirectX::SimpleMath;

	if (!isMoveForward) return;

	CP_Animation* pAnimation = pActionController->GetAnimation();
	if (!pAnimation) return;

	// �J�[�u����i�������擾
	float animPlayRatio = pAnimation->GetCurrentPlayRatio();
	float curveVal = progressDistanceCurve.GetValue(animPlayRatio);

	Transform& transform = pActionController->GetPlayer().GetTransform();

	// ���W�ɔ��f
	Vector3 pos = transform.GetPosition();
	pos += transform.Forward() * curveVal * atkMoveSpeed * DeltaTime();
	transform.SetPosition(pos);
}

nlohmann::json PlayerAttackState::Save()
{
	auto data = PlayerActState_Base::Save();

	data["nextCombAttack"] = nextCombAtkState;
	data["atkInfo"] = pAttackInfo->Save();
	data["atkMoveSpd"] = atkMoveSpeed;
	data["isMoveFwd"] = isMoveForward;
	data["forwardCurve"] = progressDistanceCurve.Save();

	return data;
}

void PlayerAttackState::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;
	PlayerActState_Base::Load(_data);

	LoadJsonEnum<PlayerState>("nextCombAttack", nextCombAtkState, _data);
	LoadJsonBoolean("isMoveFwd", isMoveForward, _data);
	LoadJsonFloat("atkMoveSpd", atkMoveSpeed, _data);
	nlohmann::json loadData;
	if (LoadJsonData("atkInfo", loadData, _data))
	{
		pAttackInfo->Load(loadData);
	}

	if (LoadJsonData("forwardCurve", loadData, _data))
	{
		progressDistanceCurve.Load(loadData);
	}
}

#include "pch.h"
#include "PlayerAttackState.h"
#include "PlayerActionController.h"
#include "GameObject.h"

PlayerAttackState::PlayerAttackState()
	: nextCombAtkState(PlayerState::None), senkoInputTime(0.15f), atkMoveSpeed(10.0f), isMoveForward(false)
{
	pAttackInfo = std::make_unique<HashiTaku::AttackInformation>();

	// �J�[�u�����Z�b�g
	forwardSpeedCurve.SetCurveName("AtkMoveSpd");
}

void PlayerAttackState::OnStartBehavior()
{
	// �U�������X�V
	UpdateAttackInfo();

	// �U���t���O�𗧂Ă�
	pAnimation->SetTrigger(ATTACKTRIGGER_PARAMNAME);
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
	PlayerActState_Base::TransitionCheckUpdate();

	// �L�����Z���ł��邩�H
	if (!pActionController->GetIsCanCancel()) return;

	// �U�����͂��ꂽ��X�e�[�g�J�ڂ���
	if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack, senkoInputTime))
		ChangeState(nextCombAtkState);
}

void PlayerAttackState::UpdateAttackInfo()
{
	pActionController->GetPlayer().SetAttackInfo(*pAttackInfo);
}

void PlayerAttackState::ImGuiDebug()
{
	ImGui::Checkbox("MoveFwd", &isMoveForward);
	ImGui::DragFloat("Senko", &senkoInputTime, 0.01f, 0.0f, 3.0f);
	ImGuiComboPlayerState("Next", nextCombAtkState);
	ImGui::Text("AtkInfo");
	pAttackInfo->ImGuiCall();

	if (isMoveForward)
	{
		ImGui::DragFloat("AtkMoveSpeed", &atkMoveSpeed, 0.1f);
		forwardSpeedCurve.ImGuiCall();
	}
}

void PlayerAttackState::ForwardProgressMove()
{
	using namespace DirectX::SimpleMath;

	if (!isMoveForward) return;

	// �J�[�u����i�������擾
	float animPlayRatio = pAnimation->GetCurrentPlayRatio();
	float curveVal = forwardSpeedCurve.GetValue(animPlayRatio);

	Transform& transform = pActionController->GetPlayer().GetTransform();

	// ���W�ɔ��f
	Vector3 pos = transform.GetPosition();
	pos += transform.Forward() * curveVal * atkMoveSpeed * MainApplication::DeltaTime();
	transform.SetPosition(pos);
}

nlohmann::json PlayerAttackState::Save()
{
	auto data = PlayerActState_Base::Save();

	data["nextCombAttack"] = nextCombAtkState;
	data["senko"] = senkoInputTime;
	data["atkInfo"] = pAttackInfo->Save();
	data["atkMoveSpd"] = atkMoveSpeed;
	data["isMoveFwd"] = isMoveForward;
	data["forwardCurve"] = forwardSpeedCurve.Save();

	return data;
}

void PlayerAttackState::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;
	PlayerActState_Base::Load(_data);

	LoadJsonEnum<PlayerState>("nextCombAttack", nextCombAtkState, _data);
	LoadJsonFloat("senko", senkoInputTime, _data);
	LoadJsonBoolean("isMoveFwd", isMoveForward, _data);
	LoadJsonFloat("atkMoveSpd", atkMoveSpeed, _data);
	nlohmann::json loadData;
	if (LoadJsonData("atkInfo", loadData, _data))
	{
		pAttackInfo->Load(loadData);
	}

	if (LoadJsonData("forwardCurve", loadData, _data))
	{
		forwardSpeedCurve.Load(loadData);
	}
}

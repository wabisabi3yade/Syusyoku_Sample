#include "pch.h"
#include "PlayerAttackState.h"
#include "PlayerActionController.h"
#include "GameObject.h"

namespace DXSimp = DirectX::SimpleMath;

PlayerAttackState::PlayerAttackState()
	: nextCombAtkState(PlayerState::None), atkMoveSpeed(10.0f), lookRotateSpeed(0.0f), isMoveForward(false), isAttackCollisionBefore(false)
{
	pAttackInfo = std::make_unique<HashiTaku::AttackInformation>();

	// ƒJ[ƒu–¼‚ðƒZƒbƒg
	progressDistanceCurve.SetCurveName("AtkMoveSpd");
}

void PlayerAttackState::OnStartBehavior()
{
	LookAtEnemyInstant();

	// UŒ‚î•ñ‚ðXV
	UpdateAttackInfo();

	// UŒ‚ƒtƒ‰ƒO‚ð—§‚Ä‚é
	pActionController->SetAnimationTrigger(ATTACKTRIGGER_PARAMNAME);

}

void PlayerAttackState::UpdateBehavior()
{
	// UŒ‚Žž‚É‘Oi‚·‚é
	ForwardProgressMove();
}

void PlayerAttackState::OnEndBehavior()
{
}

void PlayerAttackState::TransitionCheckUpdate()
{
	// UŒ‚“ü—Í‚³‚ê‚½‚çƒXƒe[ƒg‘JˆÚ‚·‚é
	if (GetCanCombAttack())
		ChangeState(nextCombAtkState);

	PlayerActState_Base::TransitionCheckUpdate();
}

void PlayerAttackState::UpdateAttackInfo()
{
	pActionController->GetPlayer().SetAttackInfo(*pAttackInfo);
}

void PlayerAttackState::OnStartRotate()
{
	/*if(GetInputLeftStick())*/


	// uŽž‚É“G‚ÉŒü‚¯‚é
	LookAtEnemyInstant();
}

void PlayerAttackState::LookAtEnemyInstant()
{
	CP_BattleManager* pBattle = CP_BattleManager::GetInstance();
	if (!pBattle) return;

	// “GƒŠƒXƒg‚ðŽæ“¾‚·‚é
	const auto& enemyList = pBattle->GetEnemyList();

	// “G‚ª‚¢‚È‚¢‚È‚ç
	if (static_cast<u_int>(enemyList.size()) == 0) return;

	DXSimp::Vector3 lookPos;
	// ƒ^[ƒQƒbƒgæ‚ª‚¢‚é‚È‚ç
	if (auto* pTargetObj = pActionController->GetTargetObject())
	{
		lookPos = pTargetObj->GetWorldPosByTargetObj();
	}
	else
	{
		// –{—ˆ‚Íˆê”Ô‹ß‚­‚Ì“G‚ðŽæ“¾‚·‚é
		lookPos = (*enemyList.begin())->GetWorldPosByTargetObj();
	}

	// “G‚Ì•ûŒü‚ðŒ©‚é
	Transform& trans = GetTransform();
	lookPos.y = trans.GetPosition().y;	// yŽ²‰ñ“]‚Ì‚Ý‚·‚é

	trans.LookAt(lookPos);
}

void PlayerAttackState::ImGuiDebug()
{
	PlayerActState_Base::ImGuiDebug();

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

	// ƒJ[ƒu‚©‚çi–³—¿‚ðŽæ“¾
	float animPlayRatio = pAnimation->GetCurrentPlayRatio();
	float curveVal = progressDistanceCurve.GetValue(animPlayRatio);

	Transform& transform = pActionController->GetPlayer().GetTransform();

	// À•W‚É”½‰f
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

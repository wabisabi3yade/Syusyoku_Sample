#include "pch.h"
#include "PlayerRollingMove.h"
#include "CP_Player.h"

constexpr auto ROLLING_ANIMNODE_NAME("RollingMove");	// ���[�����O�̃A�j���[�V�����m�[�h��

PlayerRollingMove::PlayerRollingMove() :
	rollingDistance(5.0f), invicibleTime(0.4f), elapsedTime(0.0f),
	prevProgressDistance(0.0f)
{
	pDistanceCurve = std::make_unique<AnimationCurve>();
}

void PlayerRollingMove::OnStartBehavior()
{
	PlayerActState_Base::OnStartBehavior();

	// ���͂̌����ɑ����Ɍ�����
	TurnInputVec();

	BeginParametar();
}

void PlayerRollingMove::UpdateBehavior()
{
	using namespace DirectX::SimpleMath;
	PlayerActState_Base::UpdateBehavior();

	// �ړ�
	Move();

	// ���G���Ԃ̍X�V
	UpdateInvicible();
}

void PlayerRollingMove::OnEndBehavior()
{
	ClearVelocity(false);
	GetPlayer().SetIsInvicible(false);
}

void PlayerRollingMove::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
{
	// �A�j���[�V�����̑J�ڐ悪�ҋ@��ԂȂ�ҋ@�ɖ߂�
	if (_toAnimNodeName == IDLE_ANIM_NAME)
		ChangeState(PlayerState::Idle);
}

void PlayerRollingMove::TurnInputVec()
{
	using namespace DirectX::SimpleMath;
	Transform& transform = GetPlayer().GetTransform();

	// �������������߂�
	Transform& camTransform = pActionController->GetCamera().GetTransform();
	Vector3 camForwardVec = camTransform.Forward();
	Vector3 camRightVec = camTransform.Right();
	Vector2 input = GetInputLeftStick();
	Vector3 turnVec;
	turnVec = camRightVec * input.x;
	turnVec += camForwardVec * input.y;
	turnVec.y = 0.0f;
	turnVec.Normalize();

	// �I�u�W�F�N�g��������
	Quaternion targetRotation = Quat::RotateToVector(turnVec);
	transform.SetRotation(targetRotation);
}

void PlayerRollingMove::BeginParametar()
{
	elapsedTime = 0.0f;
	prevProgressDistance = 0.0f;

	// ���G
	GetPlayer().SetIsInvicible(true);

	// �A�j���[�V�����̔��f
	pActionController->GetAnimation()->SetTrigger(ROLLING_ANIMPARAM);
}

void PlayerRollingMove::Move()
{
	using namespace DirectX::SimpleMath;

	float deltaTime = DeltaTime();
	if (deltaTime < Mathf::epsilon) return;

	Transform& transform = GetPlayer().GetTransform();

	// �A�j���[�V�����̍Đ���������i�ދ��������߂�
	float animPlayRatio = pActionController->GetAnimation()->GetCurrentPlayRatio();
	float curveDistance = pDistanceCurve->GetValue(animPlayRatio) * rollingDistance;

	// �O��Ƃ̍����l�����߂�
	float moveDistance = curveDistance - prevProgressDistance;

	GetRB().SetVelocity(transform.Forward() * moveDistance / deltaTime);

	// ���t���[���ׂ̈ɍX�V
	prevProgressDistance = curveDistance;
}

void PlayerRollingMove::UpdateInvicible()
{
	// ���G���ԓ�������
	elapsedTime += DeltaTime();
	CP_Player& player = GetPlayer();

	if (player.GetIsInvicible() && elapsedTime > invicibleTime)
		pActionController->GetPlayer().SetIsInvicible(false);
}

void PlayerRollingMove::ImGuiDebug()
{
	PlayerActState_Base::ImGuiDebug();

	ImGui::DragFloat("Distance", &rollingDistance, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("InvicibleTime", &invicibleTime, 0.01f, 0.0f, 5.0f);
	pDistanceCurve->ImGuiCall();
}

nlohmann::json PlayerRollingMove::Save()
{
	auto data = PlayerActState_Base::Save();

	data["rollingDistance"] = rollingDistance;
	data["invicibleTime"] = invicibleTime;
	data["speedCurve"] = pDistanceCurve->Save();

	return data;
}

void PlayerRollingMove::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;

	PlayerActState_Base::Load(_data);

	LoadJsonFloat("rollingDistance", rollingDistance, _data);
	LoadJsonFloat("invicibleTime", invicibleTime, _data);

	nlohmann::json speedCurveData;
	if (LoadJsonData("speedCurve", speedCurveData, _data))
	{
		pDistanceCurve->Load(speedCurveData);
	}
}

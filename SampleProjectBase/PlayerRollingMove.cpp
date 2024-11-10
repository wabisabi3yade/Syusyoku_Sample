#include "pch.h"
#include "PlayerRollingMove.h"
#include "CP_Player.h"

constexpr auto ROLLING_ANIMNODE_NAME("RollingMove");	// ���[�����O�̃A�j���[�V�����m�[�h��

PlayerRollingMove::PlayerRollingMove()
	: rollingDistance(5.0f), invicibleTime(0.4f)
{
	pSpeedCurve = std::make_unique<AnimationCurve>();
}

void PlayerRollingMove::OnStartBehavior()
{
	PlayerActState_Base::OnStartBehavior();

	// ���͂̌����ɑ����Ɍ�����
	TurnInputVec();

	// ����J�n���W���擾
	Transform& transform = pActionController->GetPlayer().GetTransform();
	rollingStartPos = transform.GetPosition();

	elapsedTime = 0.0f;

	// ���G
	pActionController->GetPlayer().SetIsInvicible(true);

	// �A�j���[�V�����̔��f
	pAnimation->SetTrigger(ROLLING_ANIMPARAM);
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
	pActionController->GetPlayer().SetIsInvicible(false);
}

void PlayerRollingMove::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
{
	// �A�j���[�V�����̑J�ڐ悪�ҋ@��ԂȂ�ҋ@�ɖ߂�
	if (_fromAnimNodeName == ROLLING_ANIMNODE_NAME)
		ChangeState(PlayerState::Idle);
}

void PlayerRollingMove::TurnInputVec()
{
	using namespace DirectX::SimpleMath;
	Transform& transform = pActionController->GetPlayer().GetTransform();

	// �������������߂�
	Vector3 camForwardVec = pCamera->GetTransform().Forward();
	Vector3 camRightVec = pCamera->GetTransform().Right();
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

void PlayerRollingMove::Move()
{
	using namespace DirectX::SimpleMath;

	Transform& transform = pActionController->GetPlayer().GetTransform();
	float deltaTime = MainApplication::DeltaTime();

	// �A�j���[�V�����̍Đ���������i�ދ��������߂�
	float animPlayRatio = pAnimation->GetCurrentPlayRatio();
	float moveDistance = pSpeedCurve->GetValue(animPlayRatio) * rollingDistance;

	// �O�i�����Ɉړ�����
	Vector3 pos = transform.GetPosition();
	pos = rollingStartPos + transform.Forward() * moveDistance;
	transform.SetPosition(pos);
}

void PlayerRollingMove::UpdateInvicible()
{
	// ���G���ԓ�������
	elapsedTime += MainApplication::DeltaTime();
	CP_Player& player = pActionController->GetPlayer();

	if (player.GetIsInvicible() && elapsedTime > invicibleTime)
		pActionController->GetPlayer().SetIsInvicible(false);
}

void PlayerRollingMove::ImGuiDebug()
{
	PlayerActState_Base::ImGuiDebug();

	ImGui::DragFloat("Distance", &rollingDistance, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("InvicibleTime", &invicibleTime, 0.01f, 0.0f, 5.0f);
	pSpeedCurve->ImGuiCall();
}

nlohmann::json PlayerRollingMove::Save()
{
	auto data = PlayerActState_Base::Save();

	data["rollingDistance"] = rollingDistance;
	data["invicibleTime"] = invicibleTime;
	data["speedCurve"] = pSpeedCurve->Save();

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
		pSpeedCurve->Load(speedCurveData);
	}
}

#include "pch.h"
#include "PlayerJumpState.h"
#include "CP_Player.h"

constexpr auto JUMPTRIGGER_ANIMPARAM("jumpTrigger");

namespace DXSimp = HashiTaku::DXSimp;

PlayerJumpState::PlayerJumpState() :
	jumpPower(10.0f),
	crouchTime(0.3f),
	stateElapsedTime(0.0f),
	isAlreadyJump(false)
{
}

PlayerJumpState::~PlayerJumpState()
{
}

nlohmann::json PlayerJumpState::Save()
{
	auto data = PlayerGroundState::Save();

	data["jumpPower"] = jumpPower;
	data["crouchTime"] = crouchTime;

	return data;
}

void PlayerJumpState::Load(const nlohmann::json& _data)
{
	PlayerGroundState::Load(_data);

	HashiTaku::LoadJsonFloat("jumpPower", jumpPower, _data);
	HashiTaku::LoadJsonFloat("crouchTime", crouchTime, _data);
}

void PlayerJumpState::OnStartBehavior()
{
	// ���͕����ɂ����Ɍ�����
	LookInputVectorInstant();

	// �W�����v�g���K�[������
	GetAnimation()->SetTrigger(JUMPTRIGGER_ANIMPARAM);

	GetPlayer().SetIsInvicible(true);

	stateElapsedTime = 0.0f;
	isAlreadyJump = false;
}

void PlayerJumpState::UpdateBehavior()
{
	if (isAlreadyJump) return;

	stateElapsedTime += DeltaTime();

	// �����ގ��Ԃ��߂���Ə�ɔ��
	if (stateElapsedTime > crouchTime)
	{
		OnBeginJump();
	}
}

void PlayerJumpState::OnEndBehavior()
{
	GetPlayer().SetIsInvicible(false);
}

void PlayerJumpState::OnBeginJump()
{
	isAlreadyJump = true;

	// ��ɔ��
	GetRB().AddImpulse(Vec3::Up * jumpPower);
}

void PlayerJumpState::LookInputVectorInstant()
{
	// �J�������l���������͕�����XZ����
	DXSimp::Vector2 inputFromCam = GetInputLeftStickFromCam();

	// ������0�Ȃ�
	if (inputFromCam.x < Mathf::epsilon && inputFromCam.y < Mathf::epsilon) return;

	// ���͕����̉�]�ʂ����߂�
	DXSimp::Quaternion targetRot =
		Quat::RotateToVector(DXSimp::Vector3(inputFromCam.x, 0.0f, inputFromCam.y));

	// �v���C���[�����̕����Ɍ�����
	GetMyTransform().SetRotation(targetRot);
}

void PlayerJumpState::ImGuiDebug()
{
	PlayerGroundState::ImGuiDebug();

	ImGui::DragFloat("JumpPower", &jumpPower, 0.01f, 0.0f, 10000.0f);
	ImGui::DragFloat("CrouchTime", &crouchTime, 0.001f, 0.0f, 10.0f);
}

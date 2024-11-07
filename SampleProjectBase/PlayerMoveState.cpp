#include "pch.h"
#include "PlayerMoveState.h"
#include "CP_Camera.h"
#include "CP_Player.h"
#include "GameInput.h"
#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;

constexpr float IDLE_ANIM_PLAYSPEED(1.0f);

PlayerMoveState::PlayerMoveState()
	: currentSpeed(0.0f), maxSpeed(10.0f), acceleration(40.0f), rotateSpeed(7.0f), decadeSpeed(2.0f)
{
}

nlohmann::json PlayerMoveState::Save()
{
	auto data = PlayerActState_Base::Save();

	data["maxSpeed"] = maxSpeed;
	data["acceleration"] = acceleration;
	data["decade"] = decadeSpeed;
	data["rotateSpeed"] = rotateSpeed;

	return data;
}

void PlayerMoveState::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;

	PlayerActState_Base::Load(_data);

	LoadJsonFloat("maxSpeed", maxSpeed, _data);
	LoadJsonFloat("acceleration", acceleration, _data);
	LoadJsonFloat("decade", decadeSpeed, _data);
	LoadJsonFloat("rotateSpeed", rotateSpeed, _data);
}

void PlayerMoveState::OnStartBehavior()
{

}

void PlayerMoveState::UpdateBehavior()
{
	Move();

	ApplyRootMotion();

	Rotation();
}

void PlayerMoveState::OnEndBehavior()
{
}

void PlayerMoveState::TransitionCheckUpdate()
{
	// �A�^�b�NState�ɑJ��
	if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack))
	{
		ChangeState(PlayerState::Attack11);
	}
	else if (GetCanRolling())
	{
		ChangeState(PlayerState::Rolling);
	}
	else if (currentSpeed <= Mathf::epsilon)	// �ړ����x��0�ȉ��ɂȂ��
	{
		ChangeState(PlayerState::Idle);
	}
	else if (pActionController->GetIsTargeting())
	{
		ChangeState(PlayerState::TargetMove);
	}
}

bool PlayerMoveState::IsRunning()
{
	if (currentSpeed >= Mathf::epsilon)  return true;

	return false;
}

void PlayerMoveState::ImGuiSetting()
{
	std::string text = TO_UTF8("speed") + std::to_string(currentSpeed);
	ImGui::Text(text.c_str());
	ImGui::DragFloat("maxSpeed", &maxSpeed, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("acceleration", &acceleration, 0.1f);
	ImGui::DragFloat("decadeTimes", &decadeSpeed, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("rotateSpeed", &rotateSpeed, 0.1f);
}

void PlayerMoveState::Move()
{
	float deltaTime = MainApplication::DeltaTime();
	GameObject& playerObj = pActionController->GetPlayer().GetGameObject();

	// �ړ������E�ړ��ʌ���
	Vector3 camForwardVec = pCamera->GetTransform().Forward();
	Vector3 camRightVec = pCamera->GetTransform().Right();
	Vector2 input = GetInputLeftStick();

	// �X���̑傫�����擾
	float inputMagnitude = std::min(input.Length(), 1.0f);

	// �ړ����������߂�
	if (inputMagnitude > 0.0f)
	{
		moveVector = camRightVec * input.x;
		moveVector += camForwardVec * input.y;
		moveVector.y = 0.0f;
		moveVector.Normalize();
	}

	// �ړ����x�����߂�
	float curMaxSpd = maxSpeed * inputMagnitude;
	if (currentSpeed > curMaxSpd)
	{
		// ��������������
		currentSpeed -= decadeSpeed * deltaTime;
		currentSpeed = std::max(currentSpeed, curMaxSpd);
	}
	else
	{
		// ����
		currentSpeed += acceleration * inputMagnitude * deltaTime;
		currentSpeed = std::min(currentSpeed, curMaxSpd);
	}

	// �ړ�
	Vector3 pos = playerObj.GetTransform().GetPosition();
	pos += moveVector * currentSpeed * MainApplication::DeltaTime();
	playerObj.GetTransform().SetPosition(pos);

	// �A�j���[�V�����̃u�����h�������Z�b�g
	pAnimation->SetFloat(SPEEDRATIO_PARAMNAME, currentSpeed / maxSpeed);
}

void PlayerMoveState::ApplyRootMotion()
{
	// ���[�g���[�V�����ƈړ����x����ړ����x�̍Đ����x�𒲐�����
	if (IsRunning())
	{
		float rootMotion = abs(pAnimation->GetMotionPosSpeedPerSec().z);

		if (rootMotion > Mathf::epsilon)
		{
			float animPlaySpeed = currentSpeed / rootMotion;

			pAnimation->SetCurNodePlayerSpeed(animPlaySpeed);
		}
	}
}

void PlayerMoveState::Rotation()
{
	if (!IsMoveInput()) return;

	GameObject& playerObj = pActionController->GetPlayer().GetGameObject();

	// ���͕����֌������]�ʂ����߂�
	Quaternion targetRotation = Quat::RotateToVector(moveVector);

	// ���݂̉�]�ʂ����ʐ��`��ԂŌ����Ă����B
	Quaternion rotation = playerObj.GetTransform().GetRotation();
	rotation = Quaternion::Slerp(rotation, targetRotation, rotateSpeed * MainApplication::DeltaTime());
	playerObj.GetTransform().SetRotation(rotation);
}

bool PlayerMoveState::IsMoveInput()
{
	Vector2 absInput = Vec2::Abs(GetInputLeftStick());
	if (absInput.x < Mathf::smallValue && absInput.y < Mathf::smallValue)
		return false;

	return true;
}

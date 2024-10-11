#include "pch.h"
#include "PlayerMoveState.h"

#include "CP_Camera.h"
#include "GameInput.h"
#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;

constexpr float IDLE_ANIM_PLAYSPEED(1.0f);

PlayerMoveState::PlayerMoveState()
	: PlayerActState_Base(StateType::Move), currentSpeed(0.0f), maxSpeed(10.0f), acceleration(40.0f), rotateSpeed(7.0f), decadeSpeedTimes(0.98f)
{
	pCamera = &InSceneSystemManager::GetInstance()->GetMainCamera();
}

nlohmann::json PlayerMoveState::Save()
{
	auto data =  PlayerActState_Base::Save();
	data["maxSpeed"] = maxSpeed;
	data["acceleration"] = acceleration;
	data["decade"] = decadeSpeedTimes;
	data["rotateSpeed"] = rotateSpeed;
	return data;
}

void PlayerMoveState::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;

	LoadJsonFloat("maxSpeed", maxSpeed, _data);
	LoadJsonFloat("acceleration", acceleration, _data);
	LoadJsonFloat("decade", decadeSpeedTimes, _data);
	LoadJsonFloat("rotateSpeed", rotateSpeed, _data);
}

void PlayerMoveState::OnStart()
{

}

void PlayerMoveState::Update()
{
	Move();

	Rotation();

	// �A�^�b�NState�ɑJ��
	if (GameInput::GetInstance()->GetButton(GameInput::ButtonType::Player_Attack))
	{
		ChangeState(StateType::Attack);
		pAnimation->SetTrigger(ATTACKTRIGGER_PARAMNAME);
	}

}

void PlayerMoveState::OnEnd()
{
}

bool PlayerMoveState::IsRunning()
{
	if (currentSpeed > Mathf::epsilon)  return true;

	return false;
}

void PlayerMoveState::ImGuiSetting()
{
	if (!ImGuiMethod::TreeNode("Move")) return;

	std::string text = TO_UTF8("speed") + std::to_string(currentSpeed);
	ImGui::Text(text.c_str());
	ImGui::DragFloat("maxSpeed", &maxSpeed, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("acceleration", &acceleration, 0.1f);
	ImGui::DragFloat("decadeTimes", &decadeSpeedTimes, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("rotateSpeed", &rotateSpeed, 0.1f);

	ImGui::TreePop();
}

void PlayerMoveState::Move()
{
	float deltaTime = MainApplication::DeltaTime();

	Vector3 camForwardVec = pCamera->GetTransform().Forward();
	Vector3 camRightVec = pCamera->GetTransform().Right();
	Vector2 input = InputValue();

	//Vector2 inputNo = input;
	//inputNo.Normalize();
	//HASHI_DEBUG_LOG(std::to_string(inputNo.x) + " " + std::to_string(inputNo.y));

	//float mag = input.Length();
	//float magNo = inputNo.Length();

	//if (magNo < Mathf::epsilon)
	//	magNo = Mathf::epsilon;

	//float ratio = mag / magNo;
	//

	// �ړ������E�ړ��ʌ���
	moveVector = camRightVec * input.x;
	moveVector += camForwardVec * input.y;
	moveVector.y = 0.0f;

	Vector3 moveSpeed = moveVector * maxSpeed;
	currentSpeed = moveSpeed.Length();

	//float curMax = Mathf::Lerp(0.0f, maxSpeed, input.Length());

	//if (currentSpeed <= curMax)
	//{
	//	currentSpeed += acceleration * deltaTime;
	//	currentSpeed = std::min(currentSpeed, curMax);
	//}
	//else
	//{
	//	currentSpeed *= decadeSpeedTimes;
	//}

	// �ړ�
	Vector3 pos = pPlayerObject->GetTransform().GetPosition();
	pos += moveVector * maxSpeed * MainApplication::DeltaTime();
	pPlayerObject->GetTransform().SetPosition(pos);

	// �A�j���[�V�����̃u�����h�������Z�b�g
	pAnimation->SetFloat(SPEEDRATIO_PARAMNAME, currentSpeed / maxSpeed);

	// ���[�g���[�V�����ƈړ����x����ړ����x�̍Đ����x�𒲐�����
	/*if (IsRunning())
	{
		float rootMotion = abs(pAnimation->GetMotionPosSpeedPerSec().z);

		if (rootMotion < Mathf::epsilon)
			rootMotion = Mathf::epsilon;

		float animPlaySpeed = currentSpeed / rootMotion;

		pAnimation->SetCurPlayerSpeed(animPlaySpeed);
	}
	else
	{
		pAnimation->SetCurPlayerSpeed(IDLE_ANIM_PLAYSPEED);
	}*/

}

DirectX::SimpleMath::Vector3 PlayerMoveState::MoveVector()
{
	return moveVector;
}

void PlayerMoveState::Rotation()
{
	if (!IsMoveInput()) return;

	// ���͕����֌������]�ʂ����߂�
	Quaternion targetRotation = Quat::RotateToVector(moveVector);

	// ���݂̉�]�ʂ����ʐ��`��ԂŌ����Ă����B
	Quaternion rotation = pPlayerObject->GetTransform().GetRotation();
	rotation = Quaternion::Slerp(rotation, targetRotation, rotateSpeed * MainApplication::DeltaTime());
	pPlayerObject->GetTransform().SetRotation(rotation);
}

Vector2 PlayerMoveState::InputValue()
{
	GameInput* input = GameInput::GetInstance();
	return input->GetValue(GameInput::ValueType::Player_Move);
}

bool PlayerMoveState::IsMoveInput()
{
	Vector2 absInput = Vec2::Abs(InputValue());
	if (absInput.x < Mathf::smallValue && absInput.y < Mathf::smallValue)
		return false;

	return true;
}

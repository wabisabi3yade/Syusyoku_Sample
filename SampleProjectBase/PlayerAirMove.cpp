#include "pch.h"
#include "PlayerAirMove.h"
#include "PlayerAirActionController.h"

namespace HashiTaku
{
	PlayerAirMove::PlayerAirMove() :
		currentSpeed(0.0f),
		maxSpeed(10.0f),
		acceleration(40.0f),
		vectorRotateSpeed(7.0f),
		decadeSpeed(2.0f),
		enterJumpSpeed(0.0f),
		maxJumpUpTime(0.0f)
	{
	}

	json PlayerAirMove::Save()
	{
		auto data = PlayerActState_Base::Save();

		data["maxSpeed"] = maxSpeed;
		data["acceleration"] = acceleration;
		data["decade"] = decadeSpeed;
		data["rotateSpeed"] = vectorRotateSpeed;

		return data;
	}

	void PlayerAirMove::Load(const json& _data)
	{
		using namespace HashiTaku;

		PlayerActState_Base::Load(_data);

		LoadJsonFloat("maxSpeed", maxSpeed, _data);
		LoadJsonFloat("acceleration", acceleration, _data);
		LoadJsonFloat("decade", decadeSpeed, _data);
		LoadJsonFloat("rotateSpeed", vectorRotateSpeed, _data);
	}

	void PlayerAirMove::OnStartBehavior()
	{
		// �X�e�[�g�J�n���̃W�����v���x���擾
		enterJumpSpeed = GetRB().GetVelocity().y;
		HASHI_DEBUG_LOG(std::to_string(enterJumpSpeed));

		moveVector = DXSimp::Vector3::Zero;
	}

	void PlayerAirMove::UpdateBehavior()
	{
		Move();

		//	Rotation();
	}

	void PlayerAirMove::Move()
	{
		float deltaSpeed = GetDeltaSpeed();

		// �ړ������E�ړ��ʌ���
		Transform& camTransform = pActionController->GetCamera().GetTransform();
		DXSimp::Vector3 camForwardVec = camTransform.Forward();
		DXSimp::Vector3 camRightVec = camTransform.Right();
		DXSimp::Vector2 input = GetInputLeftStick();

		// �X���̑傫�����擾
		float inputMagnitude = std::min(input.Length(), 1.0f);

		// �ړ����������߂�
		if (inputMagnitude >= Mathf::epsilon)
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
			currentSpeed -= decadeSpeed * DeltaTime();
			currentSpeed = std::max(currentSpeed, curMaxSpd);
		}
		else
		{
			// ����
			currentSpeed += acceleration * inputMagnitude;
			currentSpeed = std::min(currentSpeed, curMaxSpd);
		}

		DXSimp::Vector3 moveSpeed = moveVector * currentSpeed * deltaSpeed;

		CP_RigidBody& rb = GetRB();
		moveSpeed.y = rb.GetVelocity().y;

		// �ړ�
		rb.SetVelocity(moveSpeed);

		// �A�j���[�V�����̃u�����h�������Z�b�g
		pActionController->SetAnimationFloat(SPEEDRATIO_PARAMNAME, currentSpeed / maxSpeed);
	}

	void PlayerAirMove::Rotation()
	{
		//if (!isStickInput) return;	// �X�e�B�b�N���͂���Ă��Ȃ��Ȃ�

		Transform& transform = GetMyTransform();

		// ���͕����֌������]�ʂ����߂�
		DXSimp::Quaternion targetRotation = Quat::RotateToVector(moveVector);

		// ���݂̉�]�ʂ����ʐ��`��ԂŌ����Ă����B
		DXSimp::Quaternion rotation = transform.GetRotation();
		rotation = DXSimp::Quaternion::Slerp(rotation, targetRotation, vectorRotateSpeed * DeltaTime());
		transform.SetRotation(rotation);
	}

	void PlayerAirMove::ImGuiDebug()
	{
		PlayerAirState::ImGuiDebug();

		std::string text = TO_UTF8("speed") + std::to_string(currentSpeed);
		ImGui::Text(text.c_str());
		ImGui::DragFloat("maxSpeed", &maxSpeed, 0.1f, 0.0f, 1000.0f);
		ImGui::DragFloat("acceleration", &acceleration, 0.1f);
		ImGui::DragFloat("decadeTimes", &decadeSpeed, 0.1f, 0.0f, 1000.0f);
		ImGui::DragFloat("rotateSpeed", &vectorRotateSpeed, 0.1f);
	}
}
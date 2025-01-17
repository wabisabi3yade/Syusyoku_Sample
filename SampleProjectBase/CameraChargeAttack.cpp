#include "pch.h"
#include "CameraChargeAttack.h"
#include "CameraMoveController.h"

namespace HashiTaku
{
	CameraChargeAttack::CameraChargeAttack() :
		chargeTargetFov(30.0f),
		moveTargetFovTime(2.0f),
		elapsedMoveFovTime(0.0f),
		fovOnBeginMove(45.0f),
		moveFovEase(EaseKind::Linear)
	{
	}

	void CameraChargeAttack::OnStartBehavior()
	{
		CameraFollowMove::OnStartBehavior();

		BeginMoveFov(chargeTargetFov, moveTargetFovTime);

		// �`���[�W���̔��U�����J�n����
		calcChargeingShake.BeginShake(chargeingShakeParam);
	}

	void CameraChargeAttack::UpdateBehavior()
	{
		CameraFollowMove::UpdateBehavior();

		// ����p����������
		UpdateFovMove();

		// �`���[�W���̔��U��
		ChargeShakeUpdate();
	}

	void CameraChargeAttack::OnEndBehavior()
	{
	}

	void CameraChargeAttack::UpdateFovMove()
	{
		elapsedMoveFovTime += DeltaTime();

		float ratio = std::min(elapsedMoveFovTime / moveTargetFovTime, 1.0f);
		ratio = Easing::EaseValue(ratio, moveFovEase);
		float curFov = Mathf::Lerp(fovOnBeginMove, chargeTargetFov, ratio);

		// ����p���Z�b�g
		pCamController->SetFov(curFov);
	}

	void CameraChargeAttack::BeginMoveFov(float _targetFov, float _moveTime)
	{
		chargeTargetFov = _targetFov;
		moveTargetFovTime = std::max(_moveTime, Mathf::epsilon);
		fovOnBeginMove = pCamController->GetFov();

		// �o�ߎ��Ԃ����Z�b�g
		elapsedMoveFovTime = 0.0f;
	}

	void CameraChargeAttack::EndChargingShake()
	{
		// �`���[�W���̃V�F�C�N���I��
		calcChargeingShake.EndShake();
	}

	json CameraChargeAttack::Save()
	{
		auto data = CameraFollowMove::Save();

		data["moveFovTime"] = moveTargetFovTime;
		data["targetFov"] = chargeTargetFov;
		data["moveEase"] = moveFovEase;
		data["chargeShake"] = chargeingShakeParam.Save();

		return data;
	}

	void CameraChargeAttack::Load(const json& _data)
	{
		CameraFollowMove::Load(_data);

		LoadJsonFloat("moveFovTime", moveTargetFovTime, _data);
		LoadJsonFloat("targetFov", chargeTargetFov, _data);
		LoadJsonEnum<EaseKind>("moveEase", moveFovEase, _data);

		json shakeData;
		if (LoadJsonData("chargeShake", shakeData, _data))
		{
			chargeingShakeParam.Load(shakeData);
		}
	}

	void CameraChargeAttack::ChargeShakeUpdate()
	{
		if (!calcChargeingShake.GetIsShaking()) return;

		// �`���[�W���̃V�F�C�N�����߂�
		DXSimp::Vector3 shakeOffset;
		calcChargeingShake.CalcurateVector(DeltaTime(), shakeOffset);

		// �J�����̌������l���������[���h��Ԃ̗h������߂āA�x�[�X���W�ɉ��Z
		const Transform& camTrans = GetCamera().GetTransform();
		DXSimp::Vector3 basePos = GetBasePosition();
		basePos += camTrans.Right() * shakeOffset.x +
			camTrans.Up() * shakeOffset.y +
			camTrans.Forward() * shakeOffset.z;

		SetBasePosition(basePos);
	}

	void CameraChargeAttack::ImGuiDebug()
	{
		CameraFollowMove::ImGuiDebug();

		ImGuiMethod::LineSpaceSmall();
		ImGui::Text("Charge");
		ImGui::DragFloat("FovTime", &moveTargetFovTime, 0.01f);
		ImGui::DragFloat("targetFov", &chargeTargetFov, 0.1f);
		Easing::ImGuiSelect(moveFovEase);
		chargeingShakeParam.ImGuiCall();
	}
}

#include "pch.h"
#include "CameraTargetMove.h"
#include "CameraMoveController.h"

namespace HashiTaku
{
	CameraTargetMove::CameraTargetMove() :
		targetLookRotSpeed(4.0f), horiDisLength(5.0f), followMoveSpeed(10.0f), followOffsetY(2.0f),
		targetLookOffsetY(0.0f), maxHeight(3.0f), minHeight(0.0f)
	{

	}

	json CameraTargetMove::Save()
	{
		auto data = CameraMoveState_Base::Save();

		data["LookSpeed"] = targetLookRotSpeed;
		data["Distance"] = horiDisLength;
		data["MoveSpeed"] = followMoveSpeed;
		data["FollowOffsetY"] = followOffsetY;
		data["LookOffsetY"] = targetLookOffsetY;
		data["MaxHeight"] = maxHeight;
		data["MinHeight"] = minHeight;

		return data;
	}

	void CameraTargetMove::Load(const json& _data)
	{
		CameraMoveState_Base::Load(_data);

		LoadJsonFloat("LookSpeed", targetLookRotSpeed, _data);
		LoadJsonFloat("Distance", horiDisLength, _data);
		LoadJsonFloat("MoveSpeed", followMoveSpeed, _data);
		LoadJsonFloat("FollowOffsetY", followOffsetY, _data);
		LoadJsonFloat("LookOffsetY", targetLookOffsetY, _data);
		LoadJsonFloat("MaxHeight", maxHeight, _data);
		LoadJsonFloat("MinHeight", minHeight, _data);
	}

	void CameraTargetMove::OnStartBehavior()
	{

	}

	void CameraTargetMove::UpdateBehavior()
	{
		if (!CanUpdate()) return;

		FollowMove();

		LookMove();
	}

	void CameraTargetMove::OnEndBehavior()
	{
	}

	void CameraTargetMove::CheckTransitionUpdate()
	{
		if (!pInput->GetButton(GameInput::ButtonType::Player_RockOn))
			ChangeState(CameraState::Follow);
	}

	bool CameraTargetMove::CanUpdate()
	{
		if (!pCamController->GetHaveTarget()) return false;
		return true;
	}

	void CameraTargetMove::FollowMove()
	{
		const DXSimp::Vector3& followPos = GetFollowPosition() + Vec3::Up * followOffsetY;
		const DXSimp::Vector3& lookAtPos = pCamController->GetLookAtWorldPos();

		// 追従先から見た注視先と真逆にカメラを位置させる
		DXSimp::Vector3 lookToFollowVec = followPos - lookAtPos;
		float lookToFollowVecY = lookToFollowVec.y;
		lookToFollowVec.y = 0.0f;
		lookToFollowVec.Normalize();

		DXSimp::Vector3 disFromFollow = lookToFollowVec * horiDisLength;
		disFromFollow.y = lookToFollowVecY * horiDisLength;
		disFromFollow.y = std::clamp(disFromFollow.y, minHeight, maxHeight);

		DXSimp::Vector3 targetCamPos = followPos + disFromFollow;

		// カメラを移動させる
		DXSimp::Vector3 curCamPos = GetBasePosition();
		curCamPos = DXSimp::Vector3::Lerp(curCamPos, targetCamPos, followMoveSpeed * DeltaTime());
		SetBasePosition(curCamPos);
	}

	void CameraTargetMove::LookMove()
	{
		const DXSimp::Vector3& lookAtPos = pCamController->GetLookAtWorldPos() +
			Vec3::Up * targetLookOffsetY;
		Transform& camTrans = GetCamera().GetTransform();

		// 注視先までのベクトル
		DXSimp::Vector3 targetVec = lookAtPos - GetBasePosition();
		targetVec.Normalize();

		// 回転させる
		DXSimp::Quaternion targetRot = Quat::RotateToVector(targetVec);
		DXSimp::Quaternion camRot = camTrans.GetRotation();
		camRot = DXSimp::Quaternion::Slerp(camRot, targetRot, targetLookRotSpeed * DeltaTime());
		camTrans.SetRotation(camRot);
	}

	void CameraTargetMove::ImGuiDebug()
	{
		CameraMoveState_Base::ImGuiDebug();

		ImGui::DragFloat("LookSpeed", &targetLookRotSpeed, 0.01f, 0.0f, 1000.0f);
		ImGui::DragFloat("HoriDisLength", &horiDisLength, 0.01f, 0.0f, 1000.0f);
		ImGui::DragFloat("MoveSpeed", &followMoveSpeed, 0.01f, 0.0f, 1000.0f);
		ImGui::DragFloat("FollowOffsetY", &followOffsetY, 0.01f, 0.0f, 1000.0f);
		ImGui::DragFloat("LookOffsetY", &targetLookOffsetY, 0.01f, 0.0f, 1000.0f);
		ImGui::DragFloat("MaxHeight", &maxHeight, 0.01f, 0.0f, 1000.0f);
		ImGui::DragFloat("MinHeight", &minHeight, 0.01f, 0.0f, 1000.0f);
	}
}
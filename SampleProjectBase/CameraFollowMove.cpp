#include "pch.h"
#include "CameraFollowMove.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	float CameraFollowMove::centerAngle = 90.0f;

	CameraFollowMove::CameraFollowMove() :
		idleHeight(2.0f),
		currentHeight(2.0f),
		cameraHeightMax(7.0f),
		cameraHeightMin(1.0f),
		verticalSpeed(10.0f),
		returnVertRatio(3.0f),
		rotateSpeed(120.0f),
		horiSpeedToTarget(10.0f),
		distanceHorizon(5.0f),
		lookTargetOffsetY(2.0f),
		lookSpeedRate(10.0f),
		moveFov(65.0f),
		normalFov(50.0f),
		fovChangeRate(3.0f),
		canFovChangeMovement(1.0f),
		isFollowLooking(false),
		isTargeting(false)
	{
		pInput = &InSceneSystemManager::GetInstance()->GetInput();
	}

	void CameraFollowMove::InitCameraTransform()
	{
		Transform& camTransform = GetCamera().GetTransform();

		// 高さをそろえる
		currentHeight = idleHeight;

		// 座標
		DXSimp::Vector3 initPos = GetFollowPosition();
		float rad = centerAngle * Mathf::degToRad;
		initPos.x += cos(rad) * distanceHorizon;
		initPos.y += currentHeight;
		initPos.z += sin(rad) * distanceHorizon;
		camTransform.SetPosition(initPos);
		SetBasePosition(initPos);

		// 向き
		DXSimp::Vector3 lookPos = followPos + Vec3::Up * lookTargetOffsetY;
		DXSimp::Vector3 lookVec = lookPos - initPos;
		lookVec.Normalize();
		DXSimp::Quaternion camTargetRot = Quat::RotateToVector(lookVec);
		camTransform.SetRotation(camTargetRot);
	}

	json CameraFollowMove::Save()
	{
		auto data = CameraMoveState_Base::Save();

		SaveJsonVector2("deadZone", inpDeadZone, data);
		data["idleHeight"] = idleHeight;
		data["heightMax"] = cameraHeightMax;
		data["heightMin"] = cameraHeightMin;
		data["verticalSpeed"] = verticalSpeed;
		data["returnVert"] = returnVertRatio;
		data["rotateSpeed"] = rotateSpeed;
		data["horiSpeed"] = horiSpeedToTarget;
		data["centerAngle"] = centerAngle;
		data["disHorizon"] = distanceHorizon;
		data["lookOffsetY"] = lookTargetOffsetY;
		data["lookSpeedRate"] = lookSpeedRate;
		data["moveFov"] = moveFov;
		data["normalFov"] = normalFov;
		data["fovChangeRate"] = fovChangeRate;
		data["canFovMovement"] = canFovChangeMovement;

		return data;
	}

	void CameraFollowMove::Load(const json& _data)
	{
		CameraMoveState_Base::Load(_data);

		LoadJsonVector2("deadZone", inpDeadZone, _data);
		LoadJsonFloat("idleHeight", idleHeight, _data);
		LoadJsonFloat("heightMax", cameraHeightMax, _data);
		LoadJsonFloat("heightMin", cameraHeightMin, _data);
		LoadJsonFloat("verticalSpeed", verticalSpeed, _data);
		LoadJsonFloat("returnVert", returnVertRatio, _data);
		LoadJsonFloat("rotateSpeed", rotateSpeed, _data);
		LoadJsonFloat("horiSpeed", horiSpeedToTarget, _data);
		LoadJsonFloat("centerAngle", centerAngle, _data);
		LoadJsonFloat("disHorizon", distanceHorizon, _data);
		LoadJsonFloat("lookOffsetY", lookTargetOffsetY, _data);
		LoadJsonFloat("lookSpeedRate", lookSpeedRate, _data);
		LoadJsonFloat("moveFov", moveFov, _data);
		LoadJsonFloat("normalFov", normalFov, _data);
		LoadJsonFloat("fovChangeRate", fovChangeRate, _data);
		LoadJsonFloat("canFovMovement", canFovChangeMovement, _data);
	}

	void CameraFollowMove::OnStartBehavior()
	{
		// 現在の高さを取得
		currentHeight =
			GetCamera().GetTransform().GetPosition().y -
			GetFollowPosition().y;

		// 視点移動を滑らかに合わせに行く
		isFollowLooking = true;
	}

	void CameraFollowMove::UpdateBehavior()
	{
		Transform& camTransform = GetCamera().GetTransform();
		cameraPos = GetBasePosition();
		followPos = GetFollowPosition();

		InputUpdate();

		// 縦軸の移動
		VerticalMove();

		if (isTargeting)
			TargetUpdate();
		else
			NormalUpdate();

		// カメラの座標更新する
		SetBasePosition(cameraPos);

		// 注視点更新
		LookUpdate();

		// 視野角移動
		FovUpdate();
	}

	void CameraFollowMove::NormalUpdate()
	{
		// オブジェクトを中心に回転移動
		RotationMove();
	}

	void CameraFollowMove::TargetUpdate()
	{
		const DXSimp::Vector3& followPos = GetFollowPosition();
		const DXSimp::Vector3& lookAtPos = pCamController->GetLookAtWorldPos();

		// 追従先から見た注視先と真逆にカメラを位置させる
		DXSimp::Vector3 lookToFollowVec = followPos - lookAtPos;
		lookToFollowVec.y = 0.0f;
		lookToFollowVec.Normalize();

		DXSimp::Vector3 disFromFollow = lookToFollowVec * distanceHorizon;
		disFromFollow.y = currentHeight;

		DXSimp::Vector3 targetCamPos = followPos + disFromFollow;

		// カメラを移動させる
		cameraPos = DXSimp::Vector3::Lerp(cameraPos, targetCamPos, horiSpeedToTarget * DeltaTime());

		centerAngle = atan2f(lookToFollowVec.z, lookToFollowVec.x) * Mathf::radToDeg;
	}

	void CameraFollowMove::FovUpdate()
	{
		// 追従先のオブジェクトないなら
		if (!pCamController->GetHasFollowObject()) return;

		CP_Camera& camera = GetCamera();
		float deltaTime = DeltaTime();

		// 前フレームから追従先が動いているか取得
		DXSimp::Vector3 moveDis = GetFollowPosition() - pCamController->GetPrevFollowPos();
		bool isTargetMoving = moveDis.Length() > canFovChangeMovement * deltaTime;

		float curFov = camera.GetFov();

		// 移動量によって視野角を変える
		curFov = Mathf::Lerp(curFov,
			isTargetMoving ? moveFov : normalFov,
			fovChangeRate * deltaTime);

		camera.SetFov(curFov);
	}

	void CameraFollowMove::OnEndBehavior()
	{
	}

	void CameraFollowMove::CheckTransitionUpdate()
	{
	}

	void CameraFollowMove::InputUpdate()
	{
		isTargeting = pInput->GetButton(GameInput::ButtonType::Player_RockOn);

		inputVal = pInput->GetValue(GameInput::ValueType::Camera_Move);

		int vecX = 1;
		if (inputVal.x < 0.0f)
			vecX = -1;

		int vecY = 1;
		if (inputVal.y < 0.0f)
			vecY = -1;

		inputVal = inputVal * inputVal;

		inputVal.x *= vecX;
		inputVal.y *= vecY;
	}

	void CameraFollowMove::VerticalMove()
	{
		float inputMag = inputVal.Length();
		if (abs(inputVal.y) > inpDeadZone.y)	// 入力があるなら
		{
			// 移動量を取得
			float movement = inputVal.y * verticalSpeed * DeltaTime();
			currentHeight += movement;

			// 制限の高さ内に収める
			currentHeight = std::clamp(currentHeight, cameraHeightMin, cameraHeightMax);
		}
		else
		{
			// 待機状態の高さに戻す
			currentHeight = Mathf::Lerp(currentHeight, idleHeight, returnVertRatio * DeltaTime());
		}

		// y座標を更新する
		cameraPos.y = followPos.y + currentHeight;
	}

	void CameraFollowMove::RotationMove()
	{
		float deltaTime = DeltaTime();

		// 中心角度を移動する
		centerAngle += -inputVal.x * rotateSpeed * deltaTime;
		centerAngle = Mathf::Repeat(centerAngle, Mathf::roundDeg);

		float centerRad = centerAngle * Mathf::degToRad;

		// 目標のカメラ位置を求める(XZだけ)
		DXSimp::Vector3 targetCamPos;
		targetCamPos.x = followPos.x + cos(centerRad) * distanceHorizon;
		targetCamPos.z = followPos.z + sin(centerRad) * distanceHorizon;
		targetCamPos.y = cameraPos.y;

		// 近づける
		cameraPos = DXSimp::Vector3::Lerp(cameraPos, targetCamPos, horiSpeedToTarget * deltaTime);
	}

	void CameraFollowMove::LookUpdate()
	{
		Transform& camTrans = GetCamera().GetTransform();

		// 注視する場所を見る
		DXSimp::Vector3 lookPos = followPos + Vec3::Up * lookTargetOffsetY;
		DXSimp::Vector3 lookVec = lookPos - GetBasePosition();
		lookVec.Normalize();

		// ターゲットまでの向き
		DXSimp::Quaternion camTargetRot = Quat::RotateToVector(lookVec);
		DXSimp::Quaternion lookRot;

		if (isFollowLooking)	// 滑らかに合わせているとき
		{
			lookRot = camTrans.GetRotation();
			lookRot = DXSimp::Quaternion::Slerp(lookRot, camTargetRot, lookSpeedRate * DeltaTime());

			// ほぼ同等になったなら
			if (Quat::CheckEqual(lookRot, camTargetRot))
				isFollowLooking = false;
		}
		else // 瞬時に合わせる
		{
			lookRot = camTargetRot;
		}
		
		camTrans.SetRotation(lookRot);
	}

	void CameraFollowMove::ImGuiDebug()
	{
		CameraMoveState_Base::ImGuiDebug();

		ImGui::DragFloat2("deadZone", &inpDeadZone.x, 0.01f, 0.0f, 1.0f);

		ImGui::Text("Vertical");
		ImGui::DragFloat("CurrentHeight", &currentHeight);
		ImGui::DragFloat("IdleHeight", &idleHeight, 0.01f);
		ImGui::DragFloat("MaxHeight", &cameraHeightMax, 0.01f);
		ImGui::DragFloat("MinHeight", &cameraHeightMin, 0.01f);
		ImGui::DragFloat("VertSpeed", &verticalSpeed, 0.01f);
		ImGui::DragFloat("ReturnVert", &returnVertRatio, 0.01f);

		ImGuiMethod::LineSpaceSmall();

		ImGui::Text("Rotation");
		ImGui::DragFloat("RotateSpeed", &rotateSpeed, 0.1f);
		ImGui::DragFloat("CenterAngle", &centerAngle, 0.1f);
		ImGui::DragFloat("HoriSpeed", &horiSpeedToTarget, 0.1f, 0.0f, 1000.0f);
		ImGui::DragFloat("HoriDistance", &distanceHorizon, 0.1f);

		ImGuiMethod::LineSpaceSmall();

		ImGui::Text("Look");
		ImGui::DragFloat("lookOffsetY", &lookTargetOffsetY, 0.01f);
		ImGui::DragFloat("lookRate", &lookSpeedRate, 0.01f);

		ImGuiMethod::LineSpaceSmall();

		ImGui::Text("Look");
		ImGui::DragFloat("moveFov", &moveFov, 0.01f, 0.0f, 180.0f);
		ImGui::DragFloat("normalFov", &normalFov, 0.01f, 0.0f, 180.0f);
		ImGui::DragFloat("fovRate", &fovChangeRate, 0.001f, 0.0f, 180.0f);
		ImGui::DragFloat("changeMovement", &canFovChangeMovement, 0.001f, 0.0f, 1000.0f);
	}
}
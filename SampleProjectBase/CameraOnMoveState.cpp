#include "pch.h"
#include "CameraOnMoveState.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	CameraOnMoveState::CameraOnMoveState() :
		idleHeight(2.0f),
		currentHeight(2.0f),
		cameraHeightMax(7.0f),
		cameraHeightMin(1.0f),
		verticalSpeed(10.0f),
		returnVertRatio(3.0f),
		rotateSpeed(120.0f),
		horiSpeedToTarget(10.0f),
		centerAngle(-90.0f),
		distanceHorizon(5.0f),
		lookTargetOffsetY(2.0f),
		isTargeting(false)
	{
		pInput = &InSceneSystemManager::GetInstance()->GetInput();
	}

	void CameraOnMoveState::InitCameraTransform()
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

	nlohmann::json CameraOnMoveState::Save()
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

		return data;
	}

	void CameraOnMoveState::Load(const nlohmann::json& _data)
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
	}

	void CameraOnMoveState::OnStartBehavior()
	{
	}

	void CameraOnMoveState::UpdateBehavior()
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

	}

	void CameraOnMoveState::NormalUpdate()
	{
		// オブジェクトを中心に回転移動
		RotationMove();
	}

	void CameraOnMoveState::TargetUpdate()
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

	void CameraOnMoveState::OnEndBehavior()
	{
	}

	void CameraOnMoveState::CheckTransitionUpdate()
	{
	}

	void CameraOnMoveState::InputUpdate()
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

	void CameraOnMoveState::VerticalMove()
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

	void CameraOnMoveState::RotationMove()
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

	void CameraOnMoveState::LookUpdate()
	{
		Transform& camTrans = GetCamera().GetTransform();

		// 注視する場所を見る
		DXSimp::Vector3 lookPos = followPos + Vec3::Up * lookTargetOffsetY;
		DXSimp::Vector3 lookVec = lookPos - GetBasePosition();
		lookVec.Normalize();
		DXSimp::Quaternion camTargetRot = Quat::RotateToVector(lookVec);

		camTrans.SetRotation(camTargetRot);
	}

	void CameraOnMoveState::ImGuiDebug()
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
	}
}
#include "pch.h"
#include "CameraOnMoveState.h"
#include "InSceneSystemManager.h"

namespace DXSimp = DirectX::SimpleMath;

CameraOnMoveState::CameraOnMoveState() :
	currentHeight(2.0f),
	cameraHeightMax(7.0f),
	cameraHeightMin(1.0f),
	verticalSpeed(0.0f),
	rotateSpeed(120.0f),
	horiSpeedToTarget(10.0f),
	centerAngle(-90.0f),
	distanceHorizon(5.0f),
	lookTargetOffsetY(2.0f)
{
	pInput = &InSceneSystemManager::GetInstance()->GetInput();
}

void CameraOnMoveState::InitCameraTransform()
{
	Transform& camTransform = GetCamera().GetTransform();

	// 座標
	DXSimp::Vector3 initPos = GetFollowPosition();
	float rad = centerAngle * Mathf::degToRad;
	initPos.x += cos(rad) * distanceHorizon;
	initPos.y += currentHeight;
	initPos.z += sin(rad) * distanceHorizon;
	camTransform.SetPosition(initPos);
	SetBasePosition(initPos);

	// 向き
	DXSimp::Vector3 lookPos = targetPos + Vec3::Up * lookTargetOffsetY;
	DXSimp::Vector3 lookVec = lookPos - initPos;
	lookVec.Normalize();
	DXSimp::Quaternion camTargetRot = Quat::RotateToVector(lookVec);
	camTransform.SetRotation(camTargetRot);
}

nlohmann::json CameraOnMoveState::Save()
{
	auto data = CameraMoveState_Base::Save();

	data["currentHeight"] = currentHeight;
	/*data["heightMax"] = cameraHeightMax;
	data["heightMin"] = cameraHeightMin;
	data["verticalSpeed"] = verticalSpeed;*/
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

	HashiTaku::LoadJsonFloat("currentHeight", currentHeight, _data);
	HashiTaku::LoadJsonFloat("rotateSpeed", rotateSpeed, _data);
	HashiTaku::LoadJsonFloat("horiSpeed", horiSpeedToTarget, _data);
	HashiTaku::LoadJsonFloat("centerAngle", centerAngle, _data);
	HashiTaku::LoadJsonFloat("disHorizon", distanceHorizon, _data);
	HashiTaku::LoadJsonFloat("lookOffsetY", lookTargetOffsetY, _data);
}

void CameraOnMoveState::OnStartBehavior()
{
}

void CameraOnMoveState::UpdateBehavior()
{
	Transform& camTransform = GetCamera().GetTransform();
	cameraPos = GetBasePosition();
	targetPos = GetFollowPosition();

	inputVal = pInput->GetValue(GameInput::ValueType::Camera_Move);

	// 縦軸の移動
	VerticalMove();

	// オブジェクトを中心に回転移動
	RotationMove();

	// カメラの座標更新する
	SetBasePosition(cameraPos);

	// 注視点更新
	LookUpdate();

}

void CameraOnMoveState::OnEndBehavior()
{
}

void CameraOnMoveState::VerticalMove()
{
	cameraPos.y = targetPos.y + currentHeight;

	//// 移動量を取得
	//float movement = inputVal.y * verticalSpeed * DeltaTime();
	//currentHeight += movement;

	//// 制限の高さ内に収める
	//currentHeight = std::clamp(currentHeight, cameraHeightMin, cameraHeightMax);

	//// y座標を更新する
	//cameraPos.y = targetPos.y + currentHeight;
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
	targetCamPos.x = targetPos.x + cos(centerRad) * distanceHorizon;
	targetCamPos.z = targetPos.z + sin(centerRad) * distanceHorizon;
	targetCamPos.y = cameraPos.y;

	// 近づける
	cameraPos = DXSimp::Vector3::Lerp(cameraPos, targetCamPos, horiSpeedToTarget * deltaTime);
}

void CameraOnMoveState::LookUpdate()
{
	Transform& camTrans = GetCamera().GetTransform();

	// 注視する場所を見る
	DXSimp::Vector3 lookPos = targetPos + Vec3::Up * lookTargetOffsetY;
	DXSimp::Vector3 lookVec = lookPos - GetBasePosition();
	lookVec.Normalize();
	DXSimp::Quaternion camTargetRot = Quat::RotateToVector(lookVec);

	camTrans.SetRotation(camTargetRot);
}

void CameraOnMoveState::ImGuiDebug()
{
	CameraMoveState_Base::ImGuiDebug();

	ImGui::Text("Vertical");
	ImGui::DragFloat("CurrentHeight", &currentHeight, 0.01f);

	ImGui::Text("Rotation");
	ImGui::DragFloat("RotateSpeed", &rotateSpeed, 0.1f);
	ImGui::DragFloat("CenterAngle", &centerAngle, 0.1f);
	ImGui::DragFloat("HoriSpeed", &horiSpeedToTarget, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("HoriDistance", &distanceHorizon, 0.1f);

	ImGuiMethod::LineSpaceSmall();

	ImGui::Text("Look");
	ImGui::DragFloat("lookOffsetY", &lookTargetOffsetY, 0.01f);
}

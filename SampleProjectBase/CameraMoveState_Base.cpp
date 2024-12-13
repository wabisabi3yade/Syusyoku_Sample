#include "pch.h"
#include "CameraMoveState_Base.h"
#include "CameraMoveController.h"
#include "GameObject.h"
#include "InSceneSystemManager.h"

namespace DXSimp = DirectX::SimpleMath;

constexpr float FOV_CHANGE_MOVEMENT(0.5f);

CameraMoveState_Base::CameraMoveState_Base() :
	pCamController(nullptr),
	pInput(nullptr),
	moveFov(65.0f),
	normalFov(50.0f),
	fovChangeRate(3.0f),
	canFovChangeMovement(1.0f)
{
}

void CameraMoveState_Base::Init(CameraMoveController& _cameraController)
{
	pCamController = &_cameraController;

	pInput = &InSceneSystemManager::GetInstance()->GetInput();
}

void CameraMoveState_Base::OnStart()
{
	OnStartBehavior();
}

void CameraMoveState_Base::Update()
{
	UpdateBehavior();

	FovUpdate();

	CheckTransitionUpdate();
}

void CameraMoveState_Base::OnEnd()
{
	OnEndBehavior();
}

nlohmann::json CameraMoveState_Base::Save()
{
	nlohmann::json data;
	
	data["moveFov"] = moveFov;
	data["normalFov"] = normalFov;
	data["fovChangeRate"] = fovChangeRate;
	data["canFovMovement"] = canFovChangeMovement;

	return data;
}

void CameraMoveState_Base::Load(const nlohmann::json& _data)
{
	HashiTaku::LoadJsonFloat("moveFov", moveFov, _data);
	HashiTaku::LoadJsonFloat("normalFov", normalFov, _data);
	HashiTaku::LoadJsonFloat("fovChangeRate", fovChangeRate, _data);
	HashiTaku::LoadJsonFloat("canFovMovement", canFovChangeMovement, _data);
}

void CameraMoveState_Base::CheckTransitionUpdate()
{
}

void CameraMoveState_Base::ChangeState(CameraState _camState, bool _isForce)
{
	pCamController->ChangeState(_camState, _isForce);
}

void CameraMoveState_Base::SetBasePosition(const DirectX::SimpleMath::Vector3& _basePos)
{
	pCamController->SetCameraBasePos(_basePos);
}

CP_Camera& CameraMoveState_Base::GetCamera()
{
	return pCamController->GetCamera();
}

const DirectX::SimpleMath::Vector3& CameraMoveState_Base::GetFollowPosition() const
{
	const Transform* pObj = pCamController->GetFollowTransform();

#ifdef EDIT
	if (!pObj)
	{
		return DXSimp::Vector3::Zero;
	}
#endif // EDIT

	return pObj->GetPosition();
}

const DirectX::SimpleMath::Vector3& CameraMoveState_Base::GetBasePosition() const
{
	return pCamController->GetCameraBasePos();
}

float CameraMoveState_Base::DeltaTime() const
{
	return pCamController->GetCamera().DeltaTime();
}

void CameraMoveState_Base::ImGuiDebug()
{
	ImGui::DragFloat("moveFov", &moveFov, 0.01f, 0.0f, 180.0f);
	ImGui::DragFloat("normalFov", &normalFov, 0.01f, 0.0f, 180.0f);
	ImGui::DragFloat("fovRate", &fovChangeRate, 0.001f, 0.0f, 180.0f);
	ImGui::DragFloat("changeMovement", &canFovChangeMovement, 0.001f, 0.0f, 1000.0f);
}

void CameraMoveState_Base::FovUpdate()
{
	// 追従先のオブジェクトないなら
	if (!pCamController->GetHasFollowObject()) return;

	CP_Camera& camera = GetCamera();
	float deltaTime = DeltaTime();

	// 前フレームから追従先が動いているか取得
	DXSimp::Vector3 moveDis = GetFollowPosition() - pCamController->GetPrevFollowPos();
	bool isTargetMoving = moveDis.Length() > canFovChangeMovement * deltaTime;

	if (isTargetMoving)
		HASHI_DEBUG_LOG("aaa");
		
	float curFov = camera.GetFov();

	// 移動量によって視野角を変える
	curFov = Mathf::Lerp(curFov, 
		isTargetMoving ? moveFov : normalFov,
		fovChangeRate * deltaTime);

	camera.SetFov(curFov);
}


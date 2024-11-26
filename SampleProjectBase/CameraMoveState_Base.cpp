#include "pch.h"
#include "CameraMoveState_Base.h"
#include "CameraMoveController.h"
#include "GameObject.h"
#include "InSceneSystemManager.h"

namespace DXSimp = DirectX::SimpleMath;

CameraMoveState_Base::CameraMoveState_Base() :
	pCamController(nullptr)
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

	CheckTransitionUpdate();
}

void CameraMoveState_Base::OnEnd()
{
	OnEndBehavior();
}

nlohmann::json CameraMoveState_Base::Save()
{
	return nlohmann::json();
}

void CameraMoveState_Base::CheckTransitionUpdate()
{
	/*if(pInput->GetButtonDown(GameInput::ButtonType::Player_RockOn))
	{
		ChangeState(CameraState::Target);
	}*/
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


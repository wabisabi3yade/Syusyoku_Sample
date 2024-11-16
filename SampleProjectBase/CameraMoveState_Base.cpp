#include "pch.h"
#include "CameraMoveState_Base.h"
#include "CameraMoveController.h"
#include "GameObject.h"

namespace DXSimp = DirectX::SimpleMath;

CameraMoveState_Base::CameraMoveState_Base() :
	pCamController(nullptr)
{
}

void CameraMoveState_Base::Init(CameraMoveController& _cameraController)
{
	pCamController = &_cameraController;
}

void CameraMoveState_Base::OnStart()
{
	OnStartBehavior();
}

void CameraMoveState_Base::Update()
{
	UpdateBehavior();
}

void CameraMoveState_Base::OnEnd()
{
	OnEndBehavior();
}

nlohmann::json CameraMoveState_Base::Save()
{
	return nlohmann::json();
}

CP_Camera& CameraMoveState_Base::GetCamera()
{
	return pCamController->GetCamera();
}

const DirectX::SimpleMath::Vector3& CameraMoveState_Base::GetTargetPosition() const
{
	const Transform* pObj = pCamController->GetTargetTransform();

#ifdef EDIT
	if (!pObj)
	{
		return DXSimp::Vector3::Zero;
	}
#endif // EDIT

	return pObj->GetPosition();
}

float CameraMoveState_Base::DeltaTime() const
{
	return pCamController->GetCamera().DeltaTime();
}


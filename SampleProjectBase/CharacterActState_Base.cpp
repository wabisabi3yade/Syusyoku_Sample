#include "CharacterActState_Base.h"
#include "pch.h"

namespace HashiTaku
{
	CharacterActState_Base::CharacterActState_Base() :
		pActionController(nullptr)
	{
	}

	void CharacterActState_Base::Init(IActionController& _actionController)
	{
		pActionController = &_actionController;
	}

	void CharacterActState_Base::SetVelocity(const DXSimp::Vector3& _velocity)
	{
		pActionController->SetVelocity(_velocity);
	}

	float CharacterActState_Base::DeltaTime() const
	{
		return pActionController->DeltaTime();
	}

	IActionController& CharacterActState_Base::GetActionController()
	{
		return *pActionController;
	}

	Transform& CharacterActState_Base::GetMyTransform()
	{
		return GetActionController().GetMyTransform();
	}
}

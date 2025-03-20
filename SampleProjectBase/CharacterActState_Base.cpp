#include "CharacterActState_Base.h"
#include "pch.h"
#include "CP_SoundManager.h"

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

	void CharacterActState_Base::AddImpulse(const DXSimp::Vector3& _power)
	{
		pActionController->AddImpulse(_power);
	}

	float CharacterActState_Base::DeltaTime() const
	{
		return pActionController->DeltaTime();
	}

	void CharacterActState_Base::CreateVfx(const CreateVfxInfo& _vfxInfo, const DXSimp::Vector3& _createPos)
	{
		DX11EffekseerManager::GetInstance()->Play(_vfxInfo, _createPos);
	}

	void CharacterActState_Base::CreateSoundFX(const PlaySoundParameter& _soundParam, const DXSimp::Vector3& _soundPos)
	{
		// シーン内のサウンドマネージャーを取得
		CP_SoundManager* pSoundManager = CP_SoundManager::GetInstance();
		if (!pSoundManager) return;	// ないなら処理しない

		// 再生
		pSoundManager->PlaySE(_soundParam, _soundPos);
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

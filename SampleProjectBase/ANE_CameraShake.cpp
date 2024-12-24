#include "pch.h"
#include "ANE_CameraShake.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	ANE_CameraShake::ANE_CameraShake() :
		AnimationNotifyEvent(AnimationNotify_Base::NotifyType::ANE_CameraShake)
	{
	}

	std::unique_ptr<AnimationNotify_Base> ANE_CameraShake::Clone()
	{
		return std::make_unique<ANE_CameraShake>(*this);
	}

	json ANE_CameraShake::Save()
	{
		json data = AnimationNotifyEvent::Save();

		data["shakeParam"] = shakeParameter.Save();

		return data;
	}

	void ANE_CameraShake::Load(const json& _data)
	{
		AnimationNotifyEvent::Load(_data);

		json shakeData;
		if (LoadJsonData("shakeParam", shakeData, _data))
		{
			shakeParameter.Load(shakeData);
		}
	}

	void ANE_CameraShake::OnEvent()
	{
		// �J�������擾
		CP_Camera& camera = InSceneSystemManager::GetInstance()->GetMainCamera();

		// �J�����ړ��R���|�[�l���g�擾
		CP_CameraMove* pCamMove = camera.GetGameObject().GetComponent<CP_CameraMove>();
		if (!pCamMove)
		{
			HASHI_DEBUG_LOG("CameraMove������܂���");
			return;
		}

		// �J������h�炷
		pCamMove->ShakeCamera(shakeParameter);
	}

	void ANE_CameraShake::ImGuiDebug()
	{
		AnimationNotifyEvent::ImGuiDebug();

		shakeParameter.ImGuiCall();
	}
}


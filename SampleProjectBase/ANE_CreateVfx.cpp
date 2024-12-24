#include "pch.h"
#include "ANE_CreateVfx.h"

namespace HashiTaku
{
	ANE_CreateVfx::ANE_CreateVfx() : 
		AnimationNotifyEvent(NotifyType::ANE_CreateVfx),
		pTransform(nullptr),
		vfxHandle(DX11EffecseerManager::NONE_VFX_HANDLE),
		isExitDestroy(false)
	{
	}

	std::unique_ptr<AnimationNotify_Base> ANE_CreateVfx::Clone()
	{
		return std::make_unique<ANE_CreateVfx>(*this);
	}

	void ANE_CreateVfx::SetTransform(const Transform& _transform)
	{
		pTransform = &_transform;
	}

	json ANE_CreateVfx::Save()
	{
		json data = AnimationNotifyEvent::Save();

		data["vfxInfo"] = vfxInfo.Save();
		SaveJsonVector3("offset", createOffsetPos, data);
		data["exitDestroy"] = isExitDestroy;

		return data;
	}

	void ANE_CreateVfx::Load(const json& _data)
	{
		AnimationNotifyEvent::Load(_data);

		json vfxData;
		if (LoadJsonData("vfxInfo", vfxData, _data))
		{
			vfxInfo.Load(vfxData);
		}
		LoadJsonVector3("offset", createOffsetPos, _data);
		LoadJsonBoolean("exitDestroy", isExitDestroy, _data);
	}

	void ANE_CreateVfx::OnEvent()
	{
#ifdef EDIT
		if (!pTransform)
		{
			assert(!"Transfoorm情報がありません");
			return;
		}
#endif // EDIT


		DX11EffecseerManager* pVfxManager = DX11EffecseerManager::GetInstance();

		// エフェクトの座標を求める
		DXSimp::Vector3 createPos =	pTransform->GetPosition();
		createPos += pTransform->Right() * createOffsetPos.x;
		createPos += pTransform->Up() * createOffsetPos.y;
		createPos += pTransform->Forward() * createOffsetPos.z;

		vfxHandle = pVfxManager->Play(vfxInfo, createPos);
	}

	void ANE_CreateVfx::OnTerminal()
	{
		if (isExitDestroy && vfxHandle != DX11EffecseerManager::NONE_VFX_HANDLE)
		{
			DX11EffecseerManager::GetInstance()->DestroyVfx(vfxHandle);
		}
	}

	void ANE_CreateVfx::ImGuiDebug()
	{
		AnimationNotifyEvent::ImGuiDebug();

		ImGui::Checkbox("ExitDestroy", &isExitDestroy);
		ImGui::DragFloat3("OffsetPos", &createOffsetPos.x, 0.1f);
		vfxInfo.ImGuiCall();
	}
}


#include "pch.h"
#include "CP_VisualEffect.h"
#include "DX11EffecseerManager.h"
#include "AssetGetter.h"

CP_VisualEffect::CP_VisualEffect() : pVFX(nullptr), vfxHandle(-1), isPlaying(false)
{
}

nlohmann::json CP_VisualEffect::Save()
{
	auto data = Component::Save();

	if (pVFX)
		data["vfxName"] = pVFX->GetAssetName();

	return data;
}

void CP_VisualEffect::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	std::string vfxName;
	if (HashiTaku::LoadJsonString("vfxName", vfxName, _data))
	{
		pVFX = AssetGetter::GetAsset<VisualEffect>(vfxName);
	}
}

void CP_VisualEffect::Start()
{
	// 再生開始する
	BeginPlayVFX();
}

void CP_VisualEffect::BeginPlayVFX()
{
	if (!pVFX)	// エフェクトが設定されていないなら
	{
		HASHI_DEBUG_LOG("エフェクトが再生されていません");
		return;
	}

	// 再生中なら
	if (isPlaying) return;

	// 再生させる
	DX11EffecseerManager::GetInstance()->
		Play(pVFX->GetEffect(), GetTransform().GetPosition());

	isPlaying = true;
}

void CP_VisualEffect::StopPlayVFX()
{
	isPlaying = true;
}

void CP_VisualEffect::ImGuiDebug()
{
	// エフェクトを取得
	std::string vfxName = "Null";
	if (pVFX)
		vfxName = pVFX->GetAssetName();
	if (AssetGetter::ImGuiGetCombobox<VisualEffect>("vfx", vfxName))
	{
		pVFX = AssetGetter::GetAsset<VisualEffect>(vfxName);
	}
}

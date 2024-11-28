#include "pch.h"
#include "CP_VisualEffect.h"
#include "DX11EffecseerManager.h"
#include "AssetGetter.h"
#include "GameObject.h"

CP_VisualEffect::CP_VisualEffect() : 
	pDxVfxManager(nullptr),
	pVFX(nullptr),
	vfxHandle(-1), 
	playSpeed(1.0f), 
	isPlaying(false),
	isLoop(false)
{
}

void CP_VisualEffect::Init()
{
	pDxVfxManager = DX11EffecseerManager::GetInstance();
}

void CP_VisualEffect::SetPlaySpeed(float _playSpeed)
{
	playSpeed = _playSpeed;
	pDxVfxManager->GetManager()->SetSpeed(vfxHandle, _playSpeed);	// 速度変更
}

nlohmann::json CP_VisualEffect::Save()
{
	auto data = Component::Save();

	data["isLoop"] = isLoop;
	data["playSpeed"] = playSpeed;

	if (pVFX)
		data["vfxName"] = pVFX->GetAssetName();

	return data;
}

void CP_VisualEffect::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	HashiTaku::LoadJsonBoolean("isLoop", isLoop, _data);
	HashiTaku::LoadJsonFloat("playSpeed", playSpeed, _data);

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

void CP_VisualEffect::Update()
{
	PlayEndUpdate();
}

void CP_VisualEffect::PlayEndUpdate()
{
	if (pDxVfxManager->ExistEffect(vfxHandle)) return;	// エフェクトが存在しているなら

	isPlaying = false;

	if (isLoop)
	{
		BeginPlayVFX();	// 再生しなおす
	}
	else
	{
		gameObject->SetDestroy();	// 破棄する
	}
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


	// 再生してハンドルを取得
	Transform& transform = GetTransform();
	vfxHandle = pDxVfxManager->Play(pVFX->GetEffect(),
		transform.GetPosition(),
		transform.GetScale());

	// 再生速度を変更
	SetPlaySpeed(playSpeed);

	isPlaying = true;
}

void CP_VisualEffect::StopPlayVFX()
{
	isPlaying = false;
}

void CP_VisualEffect::ImGuiDebug()
{
	// ループ
	ImGui::Checkbox("IsLoop", &isLoop);

	// 再生速度
	if (ImGui::DragFloat("Speed", &playSpeed, 0.001f, 0.0f, 100.0f))
	{
		SetPlaySpeed(playSpeed);
	}

	// エフェクトを取得
	std::string vfxName = "Null";
	if (pVFX)
		vfxName = pVFX->GetAssetName();
	if (AssetGetter::ImGuiGetCombobox<VisualEffect>("vfx", vfxName))
	{
		pVFX = AssetGetter::GetAsset<VisualEffect>(vfxName);
	}
}

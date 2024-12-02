#include "pch.h"
#include "CP_VisualEffect.h"
#include "DX11EffecseerManager.h"
#include "AssetGetter.h"
#include "GameObject.h"

CP_VisualEffect::CP_VisualEffect() :
	pDxVfxManager(nullptr),
	pVFX(nullptr),
	vfxHandle(-1),
	currentPlayFrame(0.0f),
	playStartFrame(0),
	playEndFrame(0),
	playSpeed(1.0f),
	isPlaying(false),
	isLoop(false),
	isTrimming(false)
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

void CP_VisualEffect::SetVisualEffect(const VisualEffect* _setVfx)
{
	pVFX = _setVfx;
}

nlohmann::json CP_VisualEffect::Save()
{
	auto data = Component::Save();

	data["isLoop"] = isLoop;
	data["playSpeed"] = playSpeed;
	data["isTrimming"] = isTrimming;
	if (isTrimming)
	{
		data["startFrame"] = playStartFrame;
		data["endFrame"] = playEndFrame;
	}

	if (pVFX)
		data["vfxName"] = pVFX->GetAssetName();

	return data;
}

void CP_VisualEffect::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	HashiTaku::LoadJsonBoolean("isLoop", isLoop, _data);
	HashiTaku::LoadJsonFloat("playSpeed", playSpeed, _data);
	HashiTaku::LoadJsonBoolean("isTrimming", isTrimming, _data);
	if (isTrimming)
	{
		HashiTaku::LoadJsonInteger("startFrame", playStartFrame, _data);
		HashiTaku::LoadJsonInteger("endFrame", playEndFrame, _data);
	}

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
	if (!isPlaying) return;

	TrimmingUpdate();

	PlayEndUpdate();
}

void CP_VisualEffect::TrimmingUpdate()
{
	if (!isTrimming) return;

	// 指定した終了フレームを過ぎると
	if (static_cast<int>(currentPlayFrame) >= playEndFrame)
	{
		// エフェクトを止める
		pDxVfxManager->GetManager()->StopEffect(vfxHandle);
	}

	currentPlayFrame += playSpeed;
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
	// トリミングしないなら
	if (!isTrimming) playStartFrame = 0;

	// 再生してハンドルを取得
	Transform& transform = GetTransform();
	vfxHandle = pDxVfxManager->Play(pVFX->GetEffect(),
		transform.GetPosition(),
		transform.GetScale(),
		playStartFrame);

	currentPlayFrame = static_cast<float>(playStartFrame);

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

	// 範囲指定
	ImGui::Checkbox("IsTrimming", &isTrimming);
	if (isTrimming)
		ImGui::DragIntRange2("PlayRange", &playStartFrame, &playEndFrame, 1.0f, 0, 1000);
}

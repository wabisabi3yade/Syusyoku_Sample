#include "pch.h"
#include "CP_EffectCreater.h"
#include "AssetGetter.h"
#include "SF_GameObject.h"

void CP_EffectCreater::Init()
{
	pEffectManager = DX11EffecseerManager::GetInstance();
}

void CP_EffectCreater::Create(const std::string& _vfxName,
	float _playSpeed,
	const DirectX::SimpleMath::Vector3& _pos,
	const DirectX::SimpleMath::Vector3& _scale,
	const DirectX::SimpleMath::Vector3& _angle)
{
	//// 出すエフェクトを取得する
	//const VisualEffect* pCreateVfx = AssetGetter::GetAsset<VisualEffect>(_vfxName);
	//if (!pCreateVfx)
	//{
	//	HASHI_DEBUG_LOG(_vfxName + "というエフェクトはありません");
	//	return;
	//}

	//pEffectManager->Play(pCreateVfx->GetEffect(),
	//	_playSpeed,

	//	)
}
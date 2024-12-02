#include "pch.h"
#include "CP_EffectCreater.h"
#include "AssetGetter.h"
#include "SF_GameObject.h"

void CP_EffectCreater::Create(const std::string& _vfxName, 
	const DirectX::SimpleMath::Vector3& _pos,
	const DirectX::SimpleMath::Vector3& _scale,
	const DirectX::SimpleMath::Vector3& _angle)
{
	// 出すエフェクトを取得する
	const VisualEffect* pCreateVfx = AssetGetter::GetAsset<VisualEffect>(_vfxName);
	if (!pCreateVfx)
	{
		HASHI_DEBUG_LOG(_vfxName + "というエフェクトはありません");
		return;
	}

	// エフェクトを出す
	GameObject& createGo = SceneFunction::ObjectFunc::CreateEmpty(_vfxName);
	CP_VisualEffect* pVfxComp = createGo.AddComponent<CP_VisualEffect>();
	pVfxComp->SetVisualEffect(pCreateVfx);

	Transform& t = createGo.GetTransform();
	t.SetPosition(_pos);
	t.SetScale (_scale);
	t.SetEularAngles(_angle);
}
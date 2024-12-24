#include "pch.h"
#include "VisualEffect.h"

namespace HashiTaku
{
	VisualEffect::VisualEffect() : loadScale(1.0f)
	{
	}

	VisualEffect::VisualEffect(const Effekseer::EffectRef& _effect, float _loadScale) :
		effect(_effect), loadScale(_loadScale)
	{
	}

	VisualEffect::~VisualEffect()
	{
	}

	const Effekseer::EffectRef& VisualEffect::GetEffect() const
	{
		return effect;
	}

	json VisualEffect::Save()
	{
		auto data = AssetPath_Base::Save();
		data["loadScale"] = loadScale;
		return data;
	}

	void VisualEffect::Load(const json& _data)
	{
		AssetPath_Base::Load(_data);
	}
}